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

// structures
#include <ntddkbd.h>

// my utility functions
void NOP_Toy();
int my_vsprintf(char* buffer, const char* format, va_list args);
void native_sleep(unsigned int ms);
int PrintString(char* fmt,...);
NTSTATUS OpenKeyboard(OUT PHANDLE KeyboardHandle);
NTSTATUS native_get_keyboard_input(HANDLE KeyboardHandle, CHAR *Buffer, ULONG *Length);
NTSTATUS native_get_keyboard_char(HANDLE KeyboardHandle, HANDLE EventHandle, CHAR *c);