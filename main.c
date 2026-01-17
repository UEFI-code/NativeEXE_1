#include <ntddk.h>
#include "utils.h"

void entry()
{
	UNICODE_STRING Text = RTL_CONSTANT_STRING(L"Msg From ntdll!NtDrawText\n");
    NtDrawText(&Text);
    NOP_Toy();
	PrintString("Msg From ntdll!PrintString: %d, %s\n", 233, "Hello Native World");
	// open keyboard device
	HANDLE KeyboardHandle;
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status = OpenKeyboard(&KeyboardHandle, &IoStatusBlock);
	if (!NT_SUCCESS(Status))
	{
		ULONG win32Err = RtlNtStatusToDosError(Status);
		PrintString("Failed to open keyboard device: %x\n", win32Err);
		NOP_Toy();
		native_sleep(5000);
		return;
	}
	PrintString("Successfully opened keyboard device, Handle: 0x%x\n", KeyboardHandle);
	OBJECT_ATTRIBUTES ObjAttr;
    InitializeObjectAttributes(&ObjAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	HANDLE EventHandle;
    Status = NtCreateEvent(&EventHandle, 0x1F01FF, &ObjAttr, NotificationEvent, FALSE);
    if (!NT_SUCCESS(Status))
    {
        PrintString("Failed to create event: %x\n", Status);
        return;
    }
	PrintString("Successfully created event, Handle: 0x%x\n", EventHandle);
	while (1)
	{
		PrintString("Type something: ");
		//char c;
		//Status = native_get_keyboard_char(KeyboardHandle, &IoStatusBlock, EventHandle, &c);
		char buf[64];
		Status = native_get_keyboard_str(KeyboardHandle, &IoStatusBlock, EventHandle, buf, sizeof(buf));
		if (!NT_SUCCESS(Status))
		{
			ULONG win32Err = RtlNtStatusToDosError(Status);
			PrintString("Failed to read keyboard char: %x\n", win32Err);
			NOP_Toy();
			native_sleep(5000);
			return;
		}
		PrintString("Received keyboard string: %s\n", buf);
	}
	return;
}

// link main.obj toolget.obj ntdll.lib /SUBSYSTEM:NATIVE /machine:x64 /ENTRY:entry /out:NativeEXE_1.exe

// ref https://medium.com/windows-os-internals/windows-native-api-programming-hello-world-8f256abe1c85