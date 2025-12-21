#pragma warning(push)
#pragma warning(push, 0)

#include <stdarg.h>
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

static void reverse(char* str, int len) {
    int i = 0, j = len - 1;
    while (i < j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++; j--;
    }
}

static int itoa_simple(int64_t value, char* str, int base) {
    char* ptr = str;
    int is_negative = 0;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return 1;
    }

    if (base == 10 && value < 0) {
        is_negative = 1;
        value = -value;
    }

    while (value != 0) {
        int rem = (uint64_t)value % base;
        *ptr++ = (rem > 9) ? (rem - 10 + 'a') : (rem + '0');
        value /= base;
    }

    if (is_negative) *ptr++ = '-';
    *ptr = '\0';
    reverse(str, ptr - str);
    return ptr - str;
}

int my_vsprintf(char* buffer, const char* format, va_list args)
{
    char* buf_ptr = buffer;
    const char* p = format;

    while (*p) {
        if (*p != '%') {
            *buf_ptr++ = *p++;
            continue;
        }

        p++;

        if (*p == 'd') {
            int val = va_arg(args, int);
            char numbuf[32];
            int len = itoa_simple(val, numbuf, 10);
            for (int i = 0; i < len; i++) *buf_ptr++ = numbuf[i];
        }
        else if (*p == 'x') {
            int val = va_arg(args, int);
            char numbuf[32];
            int len = itoa_simple(val, numbuf, 16);
            for (int i = 0; i < len; i++) *buf_ptr++ = numbuf[i];
        }
        else if (*p == 's') {
            char* s = va_arg(args, char*);
            while (*s) *buf_ptr++ = *s++;
        }
        else if (*p == 'c') {
            char c = (char)va_arg(args, int);
            *buf_ptr++ = c;
        }
        else {
            *buf_ptr++ = *p;
        }
        p++;
    }

    *buf_ptr = '\0';
    return buf_ptr - buffer;
}

#pragma warning(pop)