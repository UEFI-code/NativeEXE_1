#include <ntddk.h>
#include "utils.h"

 typedef struct _FILE_DIRECTORY_INFORMATION {
    ULONG         NextEntryOffset;
    ULONG         FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG         FileAttributes;
    ULONG         FileNameLength;
    WCHAR         FileName[1];
 } FILE_DIRECTORY_INFORMATION, * PFILE_DIRECTORY_INFORMATION;

 static FILE_DIRECTORY_INFORMATION dir_info_buf[128]; // heap

void list_dir(char *ascii_path)
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
    IO_STATUS_BLOCK iosb;
    NTSTATUS status = NtCreateFile(
        &hDir,
        FILE_LIST_DIRECTORY | SYNCHRONIZE,
        &oa,
        &iosb,
        NULL,
        FILE_ATTRIBUTE_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        FILE_OPEN,
        FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
    );
    if (!NT_SUCCESS(status))
    {
        PrintString("NtCreateFile failed: 0x%X\n", RtlNtStatusToDosError(status));
        return;
    }

    status = NtQueryDirectoryFile(
        hDir,
        NULL,
        NULL,
        NULL,
        &iosb,
        dir_info_buf,
        sizeof(dir_info_buf),
        FileDirectoryInformation,
        FALSE,
        NULL,
        TRUE
    );
    if (status == STATUS_NO_MORE_FILES)
    {
        PrintString("STATUS_NO_MORE_FILES\n");
        NtClose(hDir);
        return;
    }

    PFILE_DIRECTORY_INFORMATION info = dir_info_buf;
    while (1)
    {
        PrintString("AccessTime: %x, Size: %x, Attr: %x, Name: ", info->LastAccessTime.LowPart, info->EndOfFile.LowPart, info->FileAttributes);
        UNICODE_STRING UnicodeString = {.Length = (UINT16)info->FileNameLength, .MaximumLength = (UINT16)info->FileNameLength, .Buffer = info->FileName};
        NtDisplayString(&UnicodeString);
        PutChar('\n');
        if (info->NextEntryOffset == 0)
            break;
        info = (PFILE_DIRECTORY_INFORMATION)((UINT8 *)info + info->NextEntryOffset);
    }

    NtClose(hDir);
}