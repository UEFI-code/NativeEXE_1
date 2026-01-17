// ntdll functions
NTSTATUS NtDrawText(PUNICODE_STRING Text);
NTSTATUS NtDisplayString(PUNICODE_STRING DisplayString);
NTSTATUS NtDelayExecution(BOOLEAN Alertable, PLARGE_INTEGER DelayInterval);
NTSTATUS NtOpenFile(OUT PHANDLE FileHandle,
                    ACCESS_MASK DesiredAccess,
                    POBJECT_ATTRIBUTES ObjectAttributes,
                    PIO_STATUS_BLOCK IoStatusBlock,
                    ULONG ShareAccess,
                    ULONG OpenOptions);
NTSTATUS NtCreateFile(
    OUT PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength
);
NTSTATUS NtCreateEvent(
    OUT PHANDLE EventHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    EVENT_TYPE EventType,
    BOOLEAN InitialState
);
NTSTATUS NtClearEvent(
    IN HANDLE EventHandle
);
NTSTATUS NtReadFile(
    HANDLE FileHandle,
    HANDLE Event,
    PIO_APC_ROUTINE ApcRoutine,
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID Buffer,
    ULONG Length,
    PLARGE_INTEGER ByteOffset,
    PULONG Key
);
NTSTATUS NtWaitForSingleObject(
    HANDLE Handle,
    BOOLEAN Alertable,
    PLARGE_INTEGER Timeout
);
ULONG NTAPI RtlNtStatusToDosError(
    NTSTATUS Status
);
NTSTATUS NTAPI NtQueryDirectoryFile (
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ UINT32 FileInformationClass,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_opt_ PUNICODE_STRING FileName,
    _In_ BOOLEAN RestartScan
    );

VOID NtClose(IN HANDLE Handle);
VOID RtlExitUserProcess(NTSTATUS ExitStatus);
VOID NtTerminateProcess(
    IN HANDLE ProcessHandle,
    IN NTSTATUS ExitStatus
);

// structures
#include <ntddkbd.h>

// my utility functions
void NOP_Toy();
int my_vsprintf(char* buffer, const char* format, va_list args);
int my_strcmp(const char* str1, const char* str2);
int my_strncmp(const char* str1, const char* str2, size_t n);
void native_sleep(unsigned int ms);
int PrintString(char* fmt,...);
void PutChar(char c);
char scancode_2_char(UINT16 scancode);
NTSTATUS OpenKeyboard(OUT PHANDLE pKeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock);
NTSTATUS native_get_keyboard_scancode(HANDLE KeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock, HANDLE EventHandle, KEYBOARD_INPUT_DATA *pInputData);
NTSTATUS native_get_keyboard_char(HANDLE KeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock, HANDLE EventHandle, CHAR *c);
NTSTATUS native_get_keyboard_str(HANDLE KeyboardHandle, IO_STATUS_BLOCK *pIoStatusBlock, HANDLE EventHandle, CHAR *buffer, UINT32 bufferSize);
void list_dir(char *ascii_path);
void execute_command(char* command);