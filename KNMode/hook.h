#pragma once
#include "memory.h"

typedef HBRUSH(*GdiSelectBrush_t)(_In_ HDC hdc, _In_ HBRUSH  hbr);

typedef BOOL(*PatBlt_t)(_In_ HDC, _In_ int x, _In_ int y, _In_ int w, _In_ int h, _In_ DWORD);

typedef HDC(*NtUserGetDC_t)(HWND hWnd);

typedef HBRUSH(*NtGdiCreateSolidBrush_t)(_In_ COLORREF crColor, _In_opt_ HBRUSH hbr);

typedef int(*ReleaseDC_t)(HDC hDC);

typedef BOOL(*DeleteObjectApp_t)(HANDLE hobj);

typedef BOOL(*ExtTextOutW_t)(IN HDC hDC, //verified
	IN INT 	XStart,
	IN INT 	YStart,
	IN UINT 	fuOptions,
	IN OPTIONAL LPRECT 	UnsafeRect,
	IN LPWSTR 	UnsafeString,
	IN INT 	Count,
	IN OPTIONAL LPINT 	UnsafeDx,
	IN DWORD 	dwCodePage
);

namespace nullhook {
	bool call_kernel_function(void* kernelFunctionAddress);
	NTSTATUS hook_handler(PVOID calledParameter);
	INT FrameRect(HDC hDC, CONST RECT* lprc, HBRUSH hbr, int thickness);
	INT DrawLine(HDC hDC, int x, int y, int dx, int dy, HBRUSH hbr, int thickness, int x_step, int y_step);
	bool FillRect(HDC hDC, int x, int y, int w, int h, HBRUSH hbr);
	bool DrawBorderBox(HDC hDC, int x, int y, int w, int h, int thickness, HBRUSH hbr);
	PVOID AllocateVirtualMemory(SIZE_T Size);
	VOID FreeVirtualMemory(PVOID VirtualAddress, SIZE_T Size);
	BOOL extTextOutW(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx);
	BOOL extTextOutA(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPCSTR lpString, UINT cch, INT* lpDx);
}