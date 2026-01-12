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

NTSTATUS native_get_keyboard_char(HANDLE KeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock, HANDLE EventHandle, CHAR *c)
{
    NtClearEvent(EventHandle);
    pIoStatusBlock->Status = 0;
    KEYBOARD_INPUT_DATA InputData = {0};
    NTSTATUS Status = NtReadFile(
        KeyboardHandle,
        EventHandle,
        NULL, NULL,
        pIoStatusBlock,
        &InputData,
        sizeof(InputData),
        NULL,
        NULL
    );
    // if (!NT_SUCCESS(Status))
    // {
    //     return Status;
    // }
    // if (Status == STATUS_PENDING)
    // {
    //     NtWaitForSingleObject(EventHandle, FALSE, NULL);
    //     Status = IoStatusBlock.Status;
    // }
    PrintString("NtReadFile status: 0x%x\n", Status);
    while (1)
    {
        LARGE_INTEGER Timeout = {.QuadPart = 1000 * -10000LL};
        Status = NtWaitForSingleObject(EventHandle, FALSE, &Timeout);
        if (Status == STATUS_TIMEOUT)
        {
            PrintString("Timeout waiting for keyboard event.\n");
            continue;
        }
        else
        {
            PrintString("Keyboard event occurred.\n");
        }
        if (!(InputData.Flags & KEY_BREAK))
        {
            *c = (char)InputData.MakeCode;
            return STATUS_SUCCESS;
        }
    }
}