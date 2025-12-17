#include <ntddk.h>
#include "utils.h"

void entry()
{
	UNICODE_STRING Text = RTL_CONSTANT_STRING(L"Msg From ntdll!NtDrawText\n");
    NtDrawText(&Text);
    NOP_Toy();
	UNICODE_STRING DisplayString = RTL_CONSTANT_STRING(L"Msg From ntdll!NtDisplayString\n");
	NtDisplayString(&DisplayString);
	NOP_Toy();
    LARGE_INTEGER Timeout = {.QuadPart = 1 * 1000 * -10000LL};
    NtDelayExecution(FALSE, &Timeout);
	return;
}

// link main.obj toolget.obj ntdll.lib /SUBSYSTEM:NATIVE /machine:x64 /ENTRY:entry /out:NativeEXE_1.exe

// ref https://medium.com/windows-os-internals/windows-native-api-programming-hello-world-8f256abe1c85