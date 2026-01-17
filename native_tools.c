#include <ntddk.h>
#include <stdarg.h>
#include "utils.h"

void native_sleep(unsigned int ms)
{
    LARGE_INTEGER Timeout = {.QuadPart = ms * -10000LL};
    NtDelayExecution(FALSE, &Timeout);
}

int PrintString(char* fmt,...)
{
    int Len;
    char buffer[512];
    ANSI_STRING AnsiString;
    UNICODE_STRING UnicodeString;
    
    va_list ap;
    va_start(ap, fmt);
    Len = my_vsprintf(buffer, fmt, ap);
    va_end(ap);

    RtlInitAnsiString(&AnsiString, buffer);
    RtlAnsiStringToUnicodeString(&UnicodeString,
                                 &AnsiString,
                                 TRUE);
    NtDisplayString(&UnicodeString);
    RtlFreeUnicodeString(&UnicodeString);
    
    return Len;
}

void PutChar(char c)
{
    CHAR buffer[] = {c, 0};
    UNICODE_STRING UnicodeString = {.Length = 2, .MaximumLength = 2, .Buffer = (PWCH)buffer};
    NtDisplayString(&UnicodeString);
}

NTSTATUS OpenKeyboard(OUT PHANDLE pKeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock)
{
    UNICODE_STRING KeyboardName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    RtlInitUnicodeString(&KeyboardName, L"\\Device\\KeyboardClass0");
    InitializeObjectAttributes(
        &ObjectAttributes,
        &KeyboardName,
        OBJ_KERNEL_HANDLE,
        NULL,
        NULL
    );
    return NtCreateFile(
        pKeyboardHandle,
        SYNCHRONIZE | GENERIC_READ,
        &ObjectAttributes,
        pIoStatusBlock,
        NULL,
        0x80,
        0,
        FILE_OPEN,
        1,
        NULL,
        0
    );
}

NTSTATUS native_get_keyboard_scancode(HANDLE KeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock, HANDLE EventHandle, KEYBOARD_INPUT_DATA *pInputData)
{
    NtClearEvent(EventHandle);
    pIoStatusBlock->Status = 0;
    LARGE_INTEGER ByteOffset = {0};
    NTSTATUS Status = NtReadFile(
        KeyboardHandle,
        EventHandle,
        NULL, NULL,
        pIoStatusBlock,
        pInputData,
        sizeof(KEYBOARD_INPUT_DATA),
        &ByteOffset,
        NULL
    );
    if (!NT_SUCCESS(Status))
        return Status;
    //PrintString("NtReadFile succeeded: 0x%x\n", Status);
    if (Status == STATUS_PENDING)
    {
        //PrintString("Read pending, waiting for event...\n");
        NtWaitForSingleObject(EventHandle, FALSE, NULL);
    }
    //PrintString("Event signaled, Flags: 0x%x, MakeCode: 0x%x\n", pInputData->Flags, pInputData->MakeCode);
    return 0;
}

NTSTATUS native_get_keyboard_char(HANDLE KeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock, HANDLE EventHandle, CHAR *c)
{
    start:
    KEYBOARD_INPUT_DATA InputData = {0};
    NTSTATUS Status = native_get_keyboard_scancode(KeyboardHandle, pIoStatusBlock, EventHandle, &InputData);
    if (!NT_SUCCESS(Status))
        return Status;
    if (InputData.Flags == 1)
        goto start; // key release, ignore
    *c = scancode_2_char(InputData.MakeCode);
    if (*c == 0)
    {
        // special key or event
        goto start;
    }
    return 0;
}

UINT8 is_shift;
NTSTATUS native_get_keyboard_str(HANDLE KeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock, HANDLE EventHandle, CHAR *buffer, UINT32 bufferSize)
{
    buffer[bufferSize - 1] = '\0';
    UINT32 index = 0;
    while (index < bufferSize - 1)
    {
        KEYBOARD_INPUT_DATA InputData = {0};
        NTSTATUS Status = native_get_keyboard_scancode(KeyboardHandle, pIoStatusBlock, EventHandle, &InputData);
        if (!NT_SUCCESS(Status))
            return Status;
        if (InputData.Flags == 1) // key release
        {
            if (InputData.MakeCode == 0x2A) // Left Shift key
            {
                is_shift = 0;
            }
            continue;
        }
        if (InputData.MakeCode == 0x2A) // Left Shift key
        {
            is_shift = 1;
            continue;
        }
        if (InputData.MakeCode == 0x1C) // Enter key
        {
            buffer[index] = '\0';
            PutChar('\n');
            return 0;
        }
        if (InputData.MakeCode == 0x0E) // Backspace key
        {
            if (index > 0)
            {
                index--;
                PutChar('\b');
            }
            continue;
        }
        buffer[index] = scancode_2_char(InputData.MakeCode);
        if (buffer[index] == 0)
        {
            // special key or event
            continue;
        }
        PutChar(buffer[index]);
        index++;
    }
    return 0;
}