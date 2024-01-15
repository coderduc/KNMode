#include "hook.h"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driverObj, PUNICODE_STRING regPath) {
	UNREFERENCED_PARAMETER(driverObj);
	UNREFERENCED_PARAMETER(regPath);

	if (nullhook::call_kernel_function(&nullhook::hook_handler)) {
		DbgPrintEx(0, 0, "Driver loaded!\n");
		DbgPrintEx(0, 0, "NtOpenCompositionSurfaceSectionInfo function hooked\n");
		return STATUS_SUCCESS;
	}
	else {
		DbgPrintEx(0, 0, "Function Hook failed!\n");
		return STATUS_FAILED_DRIVER_ENTRY;
	}
	
}