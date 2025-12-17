#include <ntddk.h>
#include <stdarg.h>
#include "utils.h"

void sleep(unsigned int ms)
{
    LARGE_INTEGER Timeout;
    Timeout.QuadPart = -(10 * 1000 * ms); // in 100-nanosecond intervals
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