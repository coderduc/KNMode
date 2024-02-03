#include "memory.h"


PVOID get_sys_module_base(const char* moduleName) {
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	if (!bytes)
		return 0;

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x50794472); // 'PyDr'

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
		return 0;

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	PVOID module_base = 0, module_size = 0;

	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{
		if (strcmp((char*)module[i].FullPathName, moduleName) == 0)
		{
			module_base = module[i].ImageBase;
			module_size = (PVOID)module[i].ImageSize;
			break;
		}
	}

	if (modules)
		ExFreePoolWithTag(modules, 0);

	if (module_base <= 0)
		return 0;

	return module_base;
}

PVOID get_sys_module_export(const char* moduleName, LPCSTR routineName) {
	PVOID lpModule = get_sys_module_base(moduleName);

	if (!lpModule)
		return NULL;

	return RtlFindExportedRoutineByName(lpModule, routineName);
}

PVOID get_sys_routine_address(PCWSTR routine_name) {
	UNICODE_STRING name;
	RtlInitUnicodeString(&name, routine_name);
	return MmGetSystemRoutineAddress(&name);
}

PVOID get_sys_module_export(LPCWSTR module_name, LPCSTR routine_name) {
	PLIST_ENTRY module_list = reinterpret_cast<PLIST_ENTRY>(get_sys_routine_address(L"PsLoadedModuleList"));

	if (!module_list) {
		return NULL;
	}

	for (PLIST_ENTRY link = module_list; link != module_list->Blink; link = link->Flink) {
		LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(link, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		UNICODE_STRING name;
		RtlInitUnicodeString(&name, module_name);
		if (RtlEqualUnicodeString(&entry->BaseDllName, &name, TRUE)) {
			return (entry->DllBase) ? RtlFindExportedRoutineByName(entry->DllBase, routine_name) : NULL;
		}
	}
}

StructDll GetDllBase(PEPROCESS process, UNICODE_STRING moduleName)
{
	StructDll rt;
	//Getting Process Info into PPEB
	PPEB pPeb = PsGetProcessPeb(process);

	if (!pPeb) {
		rt.baseAddr = NULL;
		rt.sizeDll = NULL;
	}

	//Define
	KAPC_STATE state;

	//Attach to Process for Memory Region
	KeStackAttachProcess(process, &state);

	//Store all Modules from Memory Region into pLdr
	PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)pPeb->Ldr;

	//if pLdr is Null Detach from Memory Region
	if (!pLdr)
	{
		//Dettach from Memory Region
		KeUnstackDetachProcess(&state);
		//Return 0 if NULL
		rt.baseAddr = NULL;
		rt.sizeDll = NULL;
	}

	//Loop thru all modules till we find the one that matches the one we look for
	for (PLIST_ENTRY list = (PLIST_ENTRY)pLdr->InLoadOrderModuleList.Flink; list != &pLdr->InLoadOrderModuleList; list = (PLIST_ENTRY)list->Flink)
	{
		PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		//DbgPrintEx(0, 0, "Module name (%wZ)      Base Address: (%uX)         Size: (%ll)\n", pEntry->BaseDllName, pEntry->DllBase);
		if (RtlCompareUnicodeString(&pEntry->BaseDllName, &moduleName, TRUE) == NULL)
		{
			//Store Base Address into type baseAddress
			ULONG64 baseAddress = (ULONG64)pEntry->DllBase;
			ULONGLONG baseSize = (ULONGLONG)pEntry->SizeOfImage;
			//Dettach from Memory Region
			KeUnstackDetachProcess(&state);
			//return Base
			rt.baseAddr = baseAddress;
			rt.sizeDll = baseSize;
			return rt;
		}

	}

	//Dettach from Memory Region
	KeUnstackDetachProcess(&state);

	//Return 0
	return rt;
}

DWORD ProtectVirtualMemory(HANDLE ProcessID, PVOID Address, ULONG SizeOfMem, ULONG NewProt)
{
	NTSTATUS Status = STATUS_SUCCESS;
	KAPC_STATE State;
	DWORD oldProt = 0;
	PEPROCESS Process;
	PsLookupProcessByProcessId(ProcessID, &Process);

	KeStackAttachProcess(Process, &State);
	if (!NT_SUCCESS(ZwProtectVirtualMemory(NtCurrentProcess(), &Address, &SizeOfMem, NewProt, &oldProt))) {
		KeUnstackDetachProcess(&State);
		return STATUS_UNSUCCESSFUL;
	}
	KeUnstackDetachProcess(&State);
	return oldProt;
}



bool write_to_read_only_memory(void* address, PVOID buffer, size_t size) {

	PMDL Mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);

	if (!Mdl)
		return false;

	// Locking and mapping memory with RW-rights:
	MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	PVOID Mapping = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);
	MmProtectMdlSystemAddress(Mdl, PAGE_READWRITE);

	// Write your buffer to mapping:
	RtlCopyMemory(Mapping, buffer, size);

	// Resources freeing:
	MmUnmapLockedPages(Mapping, Mdl);
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);

	return true;
}

HANDLE get_process_id(const char* process_name) {
	ULONG buffer_size = 0;
	ZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &buffer_size);

	auto buffer = ExAllocatePoolWithTag(NonPagedPool, buffer_size, 'PyDv');
	if (!buffer) {
		DbgPrintEx(0, 0, "failed to allocate pool (get_process_id)");
		return 0;
	}

	ANSI_STRING process_name_ansi = {};
	UNICODE_STRING process_name_unicode = {};
	RtlInitAnsiString(&process_name_ansi, process_name);
	if (!NT_SUCCESS(RtlAnsiStringToUnicodeString(&process_name_unicode, &process_name_ansi, TRUE))) {
		DbgPrintEx(0, 0, "failed to convert string (get_process_id)");
		RtlFreeUnicodeString(&process_name_unicode);
		return 0;
	}

	auto process_info = (PSYSTEM_PROCESS_INFO)buffer;
	if (NT_SUCCESS(ZwQuerySystemInformation(SystemProcessInformation, process_info, buffer_size, NULL))) {
		while (process_info->NextEntryOffset) {
			if (!RtlCompareUnicodeString(&process_name_unicode, &process_info->ImageName, true)) {
				DbgPrintEx(0, 0, "process name: %wZ | process ID: %d\n", process_info->ImageName, process_info->UniqueProcessId);
				RtlFreeUnicodeString(&process_name_unicode);
				return process_info->UniqueProcessId;
			}
			process_info = (PSYSTEM_PROCESS_INFO)((BYTE*)process_info + process_info->NextEntryOffset);
		}
	}
	else {
		ExFreePoolWithTag(buffer, 'PyDv');
		return 0;
	}
}

bool read_kernel_memory(HANDLE pid, PVOID address, void* buffer, SIZE_T size) {
	if (!address || !buffer || !size)
		return false;

	SIZE_T bytes = 0;
	PEPROCESS process;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &process))) {
		DbgPrintEx(0, 0, "process lookup failed (read)");
		return false;
	}

	return MmCopyVirtualMemory(process, (PVOID)address, PsGetCurrentProcess(), buffer, size, KernelMode, &bytes) == STATUS_SUCCESS;
}

bool write_kernel_memory(HANDLE pid, PVOID address, void* buffer, SIZE_T size) {
	if (!address || !buffer || !size)
		return false;
	NTSTATUS status = STATUS_SUCCESS;
	SIZE_T bytes = 0;
	PEPROCESS process;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &process))) {
		DbgPrintEx(0, 0, "process lookup failed (write)");
		return false;
	}

	return MmCopyVirtualMemory(PsGetCurrentProcess(), (PVOID)address, process, buffer, size, KernelMode, &bytes) == STATUS_SUCCESS;
}