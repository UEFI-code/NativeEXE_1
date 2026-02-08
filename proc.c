#include <ntddk.h>
#include "utils.h"

PKUSER_SHARED_DATA SharedData = (PKUSER_SHARED_DATA)(0x7FFE0000);

void create_proc(char *ascii_path)
{
    // assume ascii_path is like "\??\C:\Windows\System32\notepad.exe"
    if (my_strncmp(ascii_path, "\\??\\", 4) != 0)
    {
        PrintString("Invalid path format. Must start with \\??\\\n");
        return;
    }
    ANSI_STRING AnsiString;
    RtlInitAnsiString(&AnsiString, ascii_path);
    UNICODE_STRING nt_path;
    RtlAnsiStringToUnicodeString(&nt_path, &AnsiString, TRUE);
    // find the image file
    PCWSTR file_part = nt_path.Buffer + nt_path.Length / 2 - 1;
    while (*file_part != L'\\')
    {
        file_part--;
    }
    file_part++;
    UNICODE_STRING exe_name;
    RtlInitUnicodeString(&exe_name, file_part);
    //wprintf(L"nt_path: %wZ, exe_name: %wZ\n", &nt_path, &exe_name);

    UNICODE_STRING cmdline;
    RtlInitUnicodeString(&cmdline, nt_path.Buffer);
    UNICODE_STRING dll_path;
    RtlInitUnicodeString(&dll_path, SharedData->NtSystemRoot);
    PRTL_USER_PROCESS_PARAMETERS proc_param;
    WCHAR Env[2] = { 0, 0 };
    NTSTATUS status = RtlCreateProcessParameters(&proc_param, &exe_name, &cmdline, &dll_path, (PUNICODE_STRING)Env, NULL, 0, 0, 0, 0);
    if (!NT_SUCCESS(status))
    {
        PrintString("RtlCreateProcessParameters failed: 0x%x\n", RtlNtStatusToDosError(status));
        return;
    }
    RTL_USER_PROCESS_INFORMATION proc_info = {0};
    status = RtlCreateUserProcess(
        &nt_path,
        OBJ_CASE_INSENSITIVE,
        proc_param,
        NULL,
        NULL,
        NULL,
        FALSE,
        NULL,
        NULL,
        &proc_info
    );
    if (!NT_SUCCESS(status))
    {
        PrintString("RtlCreateUserProcess failed: 0x%x\n", RtlNtStatusToDosError(status));
        return;
    }
    switch (proc_info.ImageInformation.SubSystemType)
    {
    case 1:
        PrintString("Creating a Native type of process.\n");
        break;
    case 2:
        PrintString("Creating a GUI type of process.\n");
        break;
    case 3:
        PrintString("Creating a Console type of process.\n");
        break;
    default:
        PrintString("Creating an Unknown type of process.\n");
        break;
    }
    status = NtResumeThread(proc_info.ThreadHandle, NULL);
    if (!NT_SUCCESS(status))
    {
        PrintString("NtResumeThread failed: 0x%x\n", RtlNtStatusToDosError(status));
        return;
    }
    PrintString("Process created successfully. Handle: 0x%x\n", proc_info.ProcessHandle);
}