#include "CppUTest/TestHarness.h"

extern "C"
{
    #include "FormatOutputSpy.h"
}

TEST_GROUP(FormatOutputSpy)
{
    void setup()
    {
        UT_PTR_SET(FormatOutput, FormatOutputSpy);
    }

    void teardown()
    {
        FormatOutputSpy_Destroy();
    }
};

/**
 * FormatOutputSpy: HelloWorld
 * - FormatOutput.h: declare FormatOutput()
 * - FormatOutput.h: convert FormatOutput() to function pointer prototype
 * - FormatOutput.c: make (*FormatOutput) point to printf()
 * - FormatOutputSpy.h: declare FormatOutputSpy()
 * - FormatOutputSpy.h: declare FormatOutputSpy_Create() and Destroy()
 * - FormatOutputSpy.h: declare FormatOutputSpy_GetOutput()
 * - FormatOutputSpy.c: Implement empty FormatOutputSpy() and _GetOutput()
 * - FormatOutputSpy.c: define static buffer, static buffer_size
 * - FormatOutputSpy.c: in _Create() define buffer_size = size+1 and make a new
 *   buffer with buffer_size using calloc().
 * - FormatOutputSpy.c: in _Destroy() free buffer and make it =0;
 * - FormatOutputSpy.c: in FormatOutputSpy():
 *      - create va_list args
 *      - Define last fixed argument: va_start(args, format)
 *      - Call vsnprintf() and store returning value in written_size
 *      - va_end
 * - FormatOutpusSpy.c: _GetOutput() returns buffer
 */
TEST(FormatOutputSpy, HelloWorld)
{
    FormatOutputSpy_Create(20);
    FormatOutput("Hello World\n");
    STRCMP_EQUAL("Hello World\n", FormatOutputSpy_GetOutput());
}

/**
 * FormatOutputSpy: LimitTheOutputBufferSize
 * The test should pass.
 */
TEST(FormatOutputSpy, LimitTheOutputBufferSize)
{
    FormatOutputSpy_Create(4);
    FormatOutput("Hello World\n");
    STRCMP_EQUAL("Hell", FormatOutputSpy_GetOutput());
}

/**
 * FormatOutputSpy: PrintMultipleTimes
 * - FormatOutputSpy.c: define static int buffer_offset, static size_t buffer_used
 * - In FormatOutputSpy_Create() reset buffer_offset=0 and buffer_used=0 to make
 *   a clean buffer when the object is created. This is only for new buffers, not
 *   for buffers that are written multiple times.
 * - in vsnprintf() use specify buffer start position (buffer+buffer_offset) and
 *   buffer available size (buffer_size-buffer_used)
 * - Update buffer_offset adding the value of written_size
 * - Update buffer_used adding the value of written_size
 */
TEST(FormatOutputSpy, PrintMultipleTimes)
{
    FormatOutputSpy_Create(25);
    FormatOutput("Hello");
    FormatOutput(", World\n");
    STRCMP_EQUAL("Hello, World\n", FormatOutputSpy_GetOutput());
}

/**
 * FormatOutputSpy: PrintMultipleOutputsPastFull
 * This test should pass
 */
TEST(FormatOutputSpy, PrintMultipleOutputsPastFull)
{
    FormatOutputSpy_Create(12);
    FormatOutput("12345");
    FormatOutput("67890");
    FormatOutput("ABCDEF");
    STRCMP_EQUAL("1234567890AB", FormatOutputSpy_GetOutput());
}