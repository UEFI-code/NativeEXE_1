#include <ntddk.h>

void NOP_Toy();
NTSTATUS NtDisplayString(PUNICODE_STRING DisplayString);
NTSTATUS NtDrawText(PUNICODE_STRING Text);
NTSTATUS NtDelayExecution(BOOLEAN Alertable, PLARGE_INTEGER DelayInterval);

void entry()
{
	NOP_Toy();
	UNICODE_STRING Text = RTL_CONSTANT_STRING(L"Hello, world!");
    NtDrawText(&Text);
    LARGE_INTEGER Timeout = {.QuadPart = 1 * 1000 * -10000LL};
    NtDelayExecution(FALSE, &Timeout);
	return;
}

// link main.obj toolget.obj ntdll.lib /SUBSYSTEM:NATIVE /machine:x64 /ENTRY:entry /out:NativeEXE_1.exe

// ref https://medium.com/windows-os-internals/windows-native-api-programming-hello-world-8f256abe1c85