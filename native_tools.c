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

NTSTATUS OpenKeyboard(OUT PHANDLE KeyboardHandle)
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
    IO_STATUS_BLOCK IoStatusBlock;
    return NtCreateFile(
        KeyboardHandle,
        SYNCHRONIZE | GENERIC_READ,
        &ObjectAttributes,
        &IoStatusBlock,
        NULL,
        0x80,
        0,
        FILE_OPEN,
        1,
        NULL,
        0
    );
}

NTSTATUS native_get_keyboard_input(HANDLE KeyboardHandle, CHAR *Buffer, ULONG *Length)
{
    NTSTATUS Status;
    IO_STATUS_BLOCK IoStatusBlock;
    KEYBOARD_INPUT_DATA InputData;
    ULONG Index = 0;

    while (1)
    {
        Status = NtReadFile(
            KeyboardHandle,
            NULL, NULL, NULL,
            &IoStatusBlock,
            &InputData,
            sizeof(InputData),
            NULL,
            NULL);

        if (Status == STATUS_PENDING)
        {
            NtWaitForSingleObject(KeyboardHandle, FALSE, NULL);
            continue;
        }

        if (!NT_SUCCESS(Status))
        {
            return Status;
        }

        if (!(InputData.Flags & KEY_BREAK))
        {
            char c = (char)InputData.MakeCode;
            if (c == 0x1C || Index >= 128 - 1)
            {
                Buffer[Index] = '\0';
                *Length = Index;
                return STATUS_SUCCESS;
            }
            Buffer[Index++] = c;
        }
    }
}

NTSTATUS native_get_keyboard_char(HANDLE KeyboardHandle, CHAR *c)
{
    NTSTATUS Status;
    IO_STATUS_BLOCK IoStatusBlock;
    KEYBOARD_INPUT_DATA InputData;

    while (1)
    {
        Status = NtReadFile(
            KeyboardHandle,
            NULL, NULL, NULL,
            &IoStatusBlock,
            &InputData,
            sizeof(InputData),
            NULL,
            NULL);

        if (Status == STATUS_PENDING)
        {
            NtWaitForSingleObject(KeyboardHandle, FALSE, NULL);
            continue;
        }

        if (!NT_SUCCESS(Status))
        {
            return Status;
        }

        if (!(InputData.Flags & KEY_BREAK))
        {
            *c = (char)InputData.MakeCode;
            return STATUS_SUCCESS;
        }
    }
}