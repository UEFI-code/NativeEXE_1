#include <ntddk.h>

void NOP_Toy();

void entry()
{
	NOP_Toy();
	DbgPrint("Hello From NativeEXE");
	return;
}

// link main.obj toolget.obj ntdll.lib /SUBSYSTEM:NATIVE /ENTRY:entry /machine:x64 /out:NativeEXE_1.exe