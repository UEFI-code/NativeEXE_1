#include <ntddk.h>

void NOP_Toy();
NTSTATUS NtDisplayString(PUNICODE_STRING DisplayString);

void entry()
{
	NOP_Toy();
	DbgPrint("Hello from NativeEXE!\n");
	NtDisplayString((PUNICODE_STRING)L"Hello from NativeEXE!\n");
	return;
}

// link main.obj toolget.obj ntdll.lib /SUBSYSTEM:NATIVE /ENTRY:entry /machine:x64 /out:NativeEXE_1.exe