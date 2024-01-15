#pragma once
#include "definitions.h"

PVOID get_sys_module_base(const char* moduleName);
PVOID get_sys_module_export(const char* moduleName, LPCSTR routineName);
PVOID get_sys_module_export(LPCWSTR module_name, LPCSTR routine_name);
PVOID get_sys_routine_address(PCWSTR routine_name);
HANDLE get_process_id(const char* process_name);
bool write_to_read_only_memory(void* address, PVOID buffer, size_t size);
bool read_kernel_memory(HANDLE pid, PVOID address, void* buffer, SIZE_T size);
bool write_kernel_memory(HANDLE pid, PVOID address, void* buffer, SIZE_T size);
StructDll GetDllBase(PEPROCESS process, UNICODE_STRING moduleName);
NTSTATUS ProtectVirtualMemory(HANDLE ProcessID, PVOID Address, ULONG SizeOfMem, ULONG NewProt);