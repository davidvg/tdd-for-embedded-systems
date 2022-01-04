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
 * - FormatOutputSpy.c: define static buffer, static buffer_size, static int 
 *   buffer_offset, static size_t buffer_used.
 * - FormatOutputSpy.c: in _Create() define buffer_size = size+1 and make a new
 *   buffer with buffer_size using calloc().
 * - FormatOutputSpy.c: in _Destroy() free buffer and make it =0;
 * - FormatOutputSpy.c: in FormatOutputSpy():
 *      - create int written_size
 *      - create va_list args
 *      - Define last fixed argument: va_start(args, format)
 *      - Call vsnprintf()
 *      - Update buffer_offset adding the value of written_size
 *      - va_end
 * - FormatOutpusSpy.c: _GetOutput() returns buffer
 */
TEST(FormatOutputSpy, HelloWorld)
{
    FormatOutputSpy_Create(20);
    FormatOutput("Hello World\n");
    STRCMP_EQUAL("Hello World\n", FormatOutputSpy_GetOutput());
}