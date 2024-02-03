#include "hook.h"

GdiSelectBrush_t NtGdiSelectBrush = NULL;
PatBlt_t NtGdiPatBlt = NULL;
NtUserGetDC_t NtUserGetDC = NULL;
NtGdiCreateSolidBrush_t NtGdiCreateSolidBrush = NULL;
ReleaseDC_t NtUserReleaseDC = NULL;
DeleteObjectApp_t NtGdiDeleteObjectApp = NULL;
ExtTextOutW_t NtGdiExtTextOutW = NULL;

typedef struct _NULL_MEMORY {
	//r/w variables
	void* buffer;
	UINT_PTR addr;
	HANDLE pid;

	//features boolean variables
	BOOLEAN getpid;
	BOOLEAN write;
	BOOLEAN read;
	BOOLEAN reg_base;
	BOOLEAN changeProtect;

	BOOLEAN draw_box;
	BOOLEAN draw_line;
	BOOLEAN fill_rect;
	BOOLEAN drawborder_box;
	BOOLEAN draw_text;

	//Draw Function
	int r, g, b, x, y, w, h, t;
	int     dx, dy;
	int     x_step, y_step;
	UINT string_length;
	LPCSTR lpString;

	//process char variables
	const char* moduleName;
	const char* processName;

	//DllStruct variables
	ULONG64 base_addr;
	ULONGLONG sizeDll;
	ULONG size;
	DWORD newProtect;
	DWORD oldProtect;
}NULL_MEMORY;

bool nullhook::call_kernel_function(void* kernel_function_address) {
	if (!kernel_function_address)
		return false;

	PVOID* hookFunction = reinterpret_cast<PVOID*>(get_sys_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", "NtOpenCompositionSurfaceSectionInfo")); //NtDxgkGetTrackedWorkloadStatistics
	
	if (!hookFunction) {
		return false;
	}

	BYTE original_func[] = { 0x4C, 0x8B, 0xDC, 0x49, 0x89, 0x5B, 0x18, 0x4D, 0x89, 0x4B, 0x20, 0x49, 0x89, 0x4B, 0x08 }; //Shellcode for the original function in dxgkrnl.sys

	BYTE shell_code_start[]
	{
		0x48, 0xB8 // 0x48 is mov, 0xBB is rax; then our function address
	};

	BYTE shell_code_end[]
	{
		0xFF, 0xE0,
		0xCC// jmp rax
	};

	RtlSecureZeroMemory(&original_func, sizeof(original_func));
	memcpy((PVOID)((ULONG_PTR)original_func), &shell_code_start, sizeof(shell_code_start));
	uintptr_t test_address = reinterpret_cast<uintptr_t>(kernel_function_address);
	memcpy((PVOID)((ULONG_PTR)original_func + sizeof(shell_code_start)), &test_address, sizeof(void*));
	memcpy((PVOID)((ULONG_PTR)original_func + sizeof(shell_code_start) + sizeof(void*)), &shell_code_end, sizeof(shell_code_end));
	write_to_read_only_memory(hookFunction, &original_func, sizeof(original_func));

	NtGdiSelectBrush = (GdiSelectBrush_t)get_sys_module_export(L"win32kfull.sys", "NtGdiSelectBrush");
	DbgPrintEx(0, 0, "[+] SysCall: NtGdiSelectBrush module_export: 0x%p \n", NtGdiSelectBrush);
	NtGdiCreateSolidBrush = (NtGdiCreateSolidBrush_t)get_sys_module_export(L"win32kfull.sys", "NtGdiCreateSolidBrush");
	DbgPrintEx(0, 0, "[+] SysCall: NtGdiCreateSolidBrush module_export: 0x%p \n", NtGdiCreateSolidBrush);
	NtGdiPatBlt = (PatBlt_t)get_sys_module_export(L"win32kfull.sys", "NtGdiPatBlt");
	DbgPrintEx(0, 0, "[+] SysCall: NtGdiPatBlt module_export: 0x%p \n", NtGdiPatBlt);
	NtUserGetDC = (NtUserGetDC_t)get_sys_module_export(L"win32kbase.sys", "NtUserGetDC");
	DbgPrintEx(0, 0, "[+] SysCall: NtUserGetDC module_export: 0x%p \n", NtUserGetDC);
	NtUserReleaseDC = (ReleaseDC_t)get_sys_module_export(L"win32kbase.sys", "NtUserReleaseDC");
	DbgPrintEx(0, 0, "[+] SysCall: NtUserReleaseDC module_export: 0x%p \n", NtUserReleaseDC);
	NtGdiDeleteObjectApp = (DeleteObjectApp_t)get_sys_module_export(L"win32kbase.sys", "NtGdiDeleteObjectApp");
	DbgPrintEx(0, 0, "[+] SysCall: NtGdiDeleteObjectApp module_export: 0x%p \n", NtGdiDeleteObjectApp);
	NtGdiExtTextOutW = (ExtTextOutW_t)get_sys_module_export(L"win32kfull.sys", "NtGdiExtTextOutW");
	DbgPrintEx(0, 0, "[+] SysCall: NtGdiExtTextOutW module_export: 0x%p \n", NtGdiExtTextOutW);

	//Checks syscall
	if (NtUserGetDC == NULL) {
		return false;
	}
	if (NtUserReleaseDC == NULL) {
		return false;
	}
	if (NtGdiCreateSolidBrush == NULL) {
		return false;
	}
	if (NtGdiDeleteObjectApp == NULL) {
		return false;
	}
	if (NtGdiSelectBrush == NULL) {
		return false;
	}
	if (NtGdiPatBlt == NULL) {
		return false;
	}
	if (NtGdiExtTextOutW == NULL) {
		return false;
	}

	return true;
}

NTSTATUS nullhook::hook_handler(PVOID calledParameter) {
	NULL_MEMORY* instr = (NULL_MEMORY*)calledParameter;
	if (instr->reg_base){
		PEPROCESS process;
		PsLookupProcessByProcessId(instr->pid, &process);
		ANSI_STRING AS;
		UNICODE_STRING ModuleName;
		RtlInitAnsiString(&AS, instr->moduleName);
		RtlAnsiStringToUnicodeString(&ModuleName, &AS, TRUE);
		StructDll dll;
		dll = GetDllBase(process, ModuleName);
		instr->base_addr = dll.baseAddr;
		instr->sizeDll = dll.sizeDll;
		RtlFreeUnicodeString(&ModuleName);
	}

	if (instr->getpid) {
		instr->pid = get_process_id(instr->processName);
	}

	if (instr->read) {
		read_kernel_memory(instr->pid, (PVOID)instr->addr, instr->buffer, instr->size);
	}

	if (instr->write) {
		write_kernel_memory(instr->pid, instr->buffer, (PVOID)instr->addr, instr->size);
	}

	if (instr->changeProtect) {
		instr->oldProtect = ProtectVirtualMemory(instr->pid, (PVOID)instr->addr, instr->size, instr->newProtect);
	}

	if (instr->draw_box) {
		HDC hdc = NtUserGetDC(NULL);
		if (!hdc)
			return STATUS_UNSUCCESSFUL;

		HBRUSH brush = NtGdiCreateSolidBrush(RGB(instr->r, instr->g, instr->b), NULL);
		if (!brush)
			return STATUS_UNSUCCESSFUL;

		RECT rect = { instr->x, instr->y, instr->x + instr->w, instr->y + instr->h };
		FrameRect(hdc, &rect, brush, instr->t);
		NtUserReleaseDC(hdc);
		NtGdiDeleteObjectApp(brush);
	}

	if (instr->draw_line)
	{
		HDC hdc = NtUserGetDC(NULL);
		if (!hdc)
			return STATUS_UNSUCCESSFUL;

		HBRUSH brush = NtGdiCreateSolidBrush(RGB(instr->r, instr->g, instr->b), NULL);
		if (!brush)
			return STATUS_UNSUCCESSFUL;


		DrawLine(hdc, instr->x, instr->y, instr->dx, instr->dy, brush, instr->t, instr->x_step, instr->y_step);
		NtUserReleaseDC(hdc);
		NtGdiDeleteObjectApp(brush);
	}

	if (instr->fill_rect) {
		HDC hdc = NtUserGetDC(NULL);
		if (!hdc)
			return STATUS_UNSUCCESSFUL;

		HBRUSH brush = NtGdiCreateSolidBrush(RGB(instr->r, instr->g, instr->b), NULL);
		if (!brush)
			return STATUS_UNSUCCESSFUL;

		FillRect(hdc, instr->x, instr->y, instr->w, instr->h, brush);
		NtUserReleaseDC(hdc);
		NtGdiDeleteObjectApp(brush);
	}

	if (instr->drawborder_box) {
		HDC hdc = NtUserGetDC(NULL);
		if (!hdc)
			return STATUS_UNSUCCESSFUL;

		HBRUSH brush = NtGdiCreateSolidBrush(RGB(instr->r, instr->g, instr->b), NULL);
		if (!brush)
			return STATUS_UNSUCCESSFUL;
		DrawBorderBox(hdc, instr->x, instr->y, instr->w, instr->h, instr->t, brush);
		NtUserReleaseDC(hdc);
		NtGdiDeleteObjectApp(brush);
	}

	if (instr->draw_text) {
		HDC hdc = NtUserGetDC(NULL);
		if (!hdc)
			return STATUS_UNSUCCESSFUL;
		extTextOutA(hdc, instr->x, instr->y, 0, NULL, instr->lpString, instr->string_length, 0);
		NtUserReleaseDC(hdc);
	}

	return STATUS_SUCCESS;
}

INT nullhook::FrameRect(HDC hDC, CONST RECT* lprc, HBRUSH hbr, int thickness) {
	HBRUSH oldbrush;
	RECT r = *lprc;

	if (!(oldbrush = NtGdiSelectBrush(hDC, hbr))) return 0;

	NtGdiPatBlt(hDC, r.left, r.top, thickness, r.bottom - r.top, PATCOPY);
	NtGdiPatBlt(hDC, r.right - thickness, r.top, thickness, r.bottom - r.top, PATCOPY);
	NtGdiPatBlt(hDC, r.left, r.top, r.right - r.left, thickness, PATCOPY);
	NtGdiPatBlt(hDC, r.left, r.bottom - thickness, r.right - r.left, thickness, PATCOPY);

	NtGdiSelectBrush(hDC, oldbrush);
	return TRUE;
}

bool nullhook::FillRect(HDC hDC, int x, int y, int w, int h, HBRUSH hbr)
{
	BOOL Ret;
	HBRUSH prevhbr = NULL;

	RECT lprc = { x, y, x + w, y + h };
	prevhbr = NtGdiSelectBrush(hDC, hbr);
	Ret = NtGdiPatBlt(hDC, lprc.left, lprc.top, lprc.right - lprc.left,
		lprc.bottom - lprc.top, PATCOPY);

	/* Select old brush */
	if (prevhbr)
		NtGdiSelectBrush(hDC, prevhbr);

	return Ret;
}

bool nullhook::DrawBorderBox(HDC hDC, int x, int y, int w, int h, int thickness, HBRUSH hbr)
{
	BOOL Ret;
	HBRUSH prevhbr = NULL;
	FillRect(hDC, x, y, w, thickness, hbr); //Top horiz line
	FillRect(hDC, x, y, thickness, h, hbr); //Left vertical line
	FillRect(hDC, (x + w), y, thickness, h, hbr); //right vertical line
	FillRect(hDC, x, y + h, w + thickness, thickness, hbr); //bottom horiz line
	return true;
}

INT nullhook::DrawLine(HDC hDC, int x, int y, int dx, int dy, HBRUSH hbr, int thickness, int x_step, int y_step) {
	HBRUSH oldbrush;

	if (!(oldbrush = NtGdiSelectBrush(hDC, hbr))) return 0;

	int ix = 0;
	int jy = 0;

	if (x * x < y * y) {
		if (y > dy) {
			for (jy = dy; jy < y; jy++) {
				ix = (jy - y_step) / x_step;
				NtGdiPatBlt(hDC, ix, jy, thickness, thickness, PATCOPY);
			}
		}
		else {
			for (jy = y; jy < dy; jy++) {
				ix = (jy - y_step) / x_step;
				NtGdiPatBlt(hDC, ix, jy, thickness, thickness, PATCOPY);
			}
		}
	}
	else {
		if (x > dx) {
			for (ix = dx; ix < x; ix++) {
				jy = x_step * ix + y_step;
				NtGdiPatBlt(hDC, ix, jy, thickness, thickness, PATCOPY);
			}
		}
		else {
			for (ix = x; ix < dx; ix++) {
				jy = x_step * ix + y_step;
				NtGdiPatBlt(hDC, ix, jy, thickness, thickness, PATCOPY);
			}
		}
	}

	NtGdiSelectBrush(hDC, oldbrush);
	return TRUE;
}

PVOID nullhook::AllocateVirtualMemory(SIZE_T Size)
{
	PVOID pMem = NULL;
	NTSTATUS statusAlloc = ZwAllocateVirtualMemory(NtCurrentProcess(), &pMem, 0, &Size, MEM_COMMIT, PAGE_READWRITE);
	//DbgPrintEx(0,0,"[+] AllocateVirtualMemory statusAlloc: %x \n", statusAlloc);
	return pMem;
}

VOID nullhook::FreeVirtualMemory(PVOID VirtualAddress, SIZE_T Size)
{
	if (MmIsAddressValid(VirtualAddress))
	{
		NTSTATUS Status = ZwFreeVirtualMemory(NtCurrentProcess(), &VirtualAddress, &Size, MEM_RELEASE);

		if (!NT_SUCCESS(Status)) {
			DbgPrintEx(0,0,"[-] GDI.cpp Warning : Released memory failed.FreeVirtualMemory Internal Function\r\n");
		}
		return;
	}
	DbgPrintEx(0, 0, "[-] GDI.cpp Warning: Released memory does not exist.FreeVirtualMemory Internal Function\r\n");
}

BOOL nullhook::extTextOutW(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx)
{
	BOOL		nRet = FALSE;
	PVOID       local_lpString = NULL;
	RECT* local_lprc = NULL;
	INT* local_lpDx = NULL;

	if (lprc != NULL)
	{
		SIZE_T Len = sizeof(RECT);
		local_lprc = (RECT*)AllocateVirtualMemory(Len);
		if (local_lprc != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lprc, Len);
				RtlCopyMemory(local_lprc, lprc, Len);
			}
			__except (1)
			{
				DbgPrintEx(0,0,"GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n");
				goto $EXIT;
			}
		}
		else
		{
			DbgPrintEx(0,0,"GDI.cpp Line local_lprc = null  Triggers An Error.ExtTextOutW Internal Function\r\n");
			goto $EXIT;
		}
	}

	if (cwc != 0)
	{
		SIZE_T     AllocSize = sizeof(WCHAR) * cwc + 1;
		local_lpString = AllocateVirtualMemory(AllocSize);

		if (local_lpString != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lpString, AllocSize);
				RtlCopyMemory(local_lpString, lpString, AllocSize);
			}
			__except (1)
			{
				DbgPrintEx(0,0,"[-] GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n");
				goto $EXIT;
			}
		}
		else
		{
			DbgPrintEx(0,0,"[-] GDI.cpp Line local_lpString = null  Triggers An Error.ExtTextOutW Internal Function\r\n");
			goto $EXIT;
		}
	}

	if (local_lpDx != NULL)
	{
		SIZE_T     AllocSize = sizeof(INT);
		local_lpDx = (INT*)AllocateVirtualMemory(AllocSize);
		if (local_lpDx != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lpString, AllocSize);
				*local_lpDx = *lpDx;
			}
			__except (1)
			{
				DbgPrintEx(0,0,"[-] GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n");
				goto $EXIT;
			}
		}
		else
		{
			DbgPrintEx(0,0,"[-] GDI.cpp Line local_lpDx = null  Triggers An Error.ExtTextOutW Internal Function\r\n");
		}
	}

	if (NtGdiExtTextOutW != NULL) {
		nRet = NtGdiExtTextOutW(hdc, x, y, fuOptions, local_lprc, (LPWSTR)local_lpString, cwc, local_lpDx, 0);
	}
	else {
		DbgPrintEx(0,0,"[-] GDI.cpp Line NtGdiExtTextOutW = NULL Triggers An Error.TextOutW Internal Function\r\n");
	}
$EXIT:
	if (lprc != NULL)
	{
		FreeVirtualMemory(lprc, sizeof(RECT));
		lprc = NULL;
	}

	if (local_lpDx != NULL)
	{
		FreeVirtualMemory(local_lpDx, sizeof(INT));
		local_lpDx = NULL;
	}

	if (local_lpString != NULL)
	{
		FreeVirtualMemory(local_lpString, cwc);
		local_lpString = NULL;
	}

	return nRet;
}

BOOL nullhook::extTextOutA(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPCSTR lpString, UINT cch, INT* lpDx)
{
	ANSI_STRING StringA;
	UNICODE_STRING StringU;
	BOOL ret;
	RtlInitAnsiString(&StringA, (LPSTR)lpString);
	RtlAnsiStringToUnicodeString(&StringU, &StringA, TRUE);
	ret = extTextOutW(hdc, x, y, fuOptions, lprc, StringU.Buffer, cch, lpDx);
	RtlFreeUnicodeString(&StringU);
	return ret;
}
