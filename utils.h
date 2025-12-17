// ntdll functions
NTSTATUS NtDrawText(PUNICODE_STRING Text);
NTSTATUS NtDisplayString(PUNICODE_STRING DisplayString);
NTSTATUS NtDelayExecution(BOOLEAN Alertable, PLARGE_INTEGER DelayInterval);

// my utility functions
void NOP_Toy();
int my_vsprintf(char *buffer, const char *format, ...);
int PrintString(char* fmt,...);