#include "FormatOutputSpy.h"

#include <stdio.h>
#include <stdarg.h>

static char * buffer = 0;
static size_t buffer_size = 0;
static int buffer_offset = 0;
static size_t buffer_used = 0;


void FormatOutputSpy_Create(int size)
{
    buffer_size = (size_t)(size + 1);
    buffer = (char *) calloc(buffer_size, sizeof(char));
    buffer_offset = 0;
    buffer_used = 0;
}

void FormatOutputSpy_Destroy(void)
{
    if (buffer == 0)
        return;
    free(buffer);
    buffer = 0;
}

int FormatOutputSpy(const char * format, ...)
{
    va_list args;
    // Specify the last fixed argument in the argument list
    va_start(args, format);
    // Write formatted data from variable argument list to sized buffer.
    int written_size = vsnprintf(buffer + buffer_offset,
                                 buffer_size - buffer_used,
                                 format,
                                 args);
    buffer_offset += written_size;
    buffer_used += (size_t)written_size;
    va_end(args);
    return 1;
}

const char * FormatOutputSpy_GetOutput(void)
{
    return buffer;
}