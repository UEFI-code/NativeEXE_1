#include <ntddk.h>
#include "utils.h"

typedef struct _OBJECT_DIRECTORY_INFORMATION {
    UNICODE_STRING Name;
    UNICODE_STRING TypeName;
} OBJECT_DIRECTORY_INFORMATION, *POBJECT_DIRECTORY_INFORMATION;

OBJECT_DIRECTORY_INFORMATION dir_info_buf[512];

void list_dev(char *ascii_path)
{
    ANSI_STRING AnsiString;
    RtlInitAnsiString(&AnsiString, ascii_path);
    UNICODE_STRING dir;
    RtlAnsiStringToUnicodeString(&dir, &AnsiString, TRUE);

    OBJECT_ATTRIBUTES oa;
    InitializeObjectAttributes(
        &oa,
        &dir,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    );
    HANDLE hDir;
    NTSTATUS status = NtOpenDirectoryObject(
        &hDir,
        DIRECTORY_QUERY,
        &oa
    );
    if (!NT_SUCCESS(status))
    {
        PrintString("NtOpenDirectoryObject failed: 0x%x\n", RtlNtStatusToDosError(status));
        return;
    }
    
    ULONG context = 0;
    ULONG retLen;
    status = NtQueryDirectoryObject(
        hDir,
        dir_info_buf,
        sizeof(dir_info_buf),
        FALSE,
        FALSE,
        &context,
        &retLen
    );
    if (!NT_SUCCESS(status))
    {
        PrintString("NtQueryDirectoryObject failed: 0x%x\n", RtlNtStatusToDosError(status));
        NtClose(hDir);
        return;
    }

    POBJECT_DIRECTORY_INFORMATION info = dir_info_buf;
    while (info->Name.Buffer)
    {
        UNICODE_STRING NameLabel = RTL_CONSTANT_STRING(L"Name: ");
        NtDisplayString(&NameLabel);
        NtDisplayString(&info->Name);
        UNICODE_STRING TypeLabel = RTL_CONSTANT_STRING(L"   Type: ");
        NtDisplayString(&TypeLabel);
        NtDisplayString(&info->TypeName);
        PutChar('\n');
        info++;
        if ((info - dir_info_buf) % 10 == 9)
        {
            UNICODE_STRING AnyKey = RTL_CONSTANT_STRING(L"...Press any key to continue...");
            NtDisplayString(&AnyKey);
            waitKey:
            KEYBOARD_INPUT_DATA InputData = {0};
            native_get_keyboard_scancode(global_KeyboardHandle, &global_IoStatusBlock, global_EventHandle, &InputData);
            if (InputData.Flags == 1)
                goto waitKey;
            PutChar('\n');
        }
    }

    NtClose(hDir);
}