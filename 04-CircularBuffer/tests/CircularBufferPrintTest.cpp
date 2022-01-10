/**
 * Tests for implementing a printing method for a CircularBuffer.
 * This method will print the content of the buffer, oldest endty to newest,
 * without disturbing the contents of the buffer.
 */

#include "CppUTest/TestHarness.h"

extern "C"
{
    #include "CircularBuffer.h"
    #include "FormatOutputSpy.h"
}

TEST_GROUP(CircularBufferPrint)
{
    CircularBuffer buffer;
    const char * expectedOutput;
    const char * actualOutput;

    void setup(void)
    {
        size_t cbSize = 10;
        int foSize = 100;

        buffer = CircularBuffer_Create(cbSize);

        UT_PTR_SET(FormatOutput, FormatOutputSpy);
        FormatOutputSpy_Create(foSize);
        actualOutput = FormatOutputSpy_GetOutput();
    }

    void teardown(void)
    {
        CircularBuffer_Destroy(&buffer);
        FormatOutputSpy_Destroy();
    }
};

/**
 * CircularBufferPrint: PrintEmpty
 * - Declare CircularBuffer_Print() and inside call FormatOutput(), with the
 *   required string as argument
 */
TEST(CircularBufferPrint, PrintEmpty)
{
    expectedOutput = "Circular buffer content:\n<>\n";
    CircularBuffer_Print(&buffer);
    STRCMP_EQUAL(expectedOutput, actualOutput);
}

/**
 * CircularBufferPrint: PrintAfterOneIsPut
 * - Split the string in CB_Print in two FormatOutput() calls, one formatting
 *   "Circular buffer content:\n<" (outputHead) and the other ">\n" (outputTail)
 *   Between both calls the content of the buffer can be formatted.
 * - Add a FormatCall() between both calls, returning the first in the buffer,
 *   after checking that the buffer is not empty. CB_Get() cannot be used,
 *   because it changes the buffer state, so its done with cb.buf[0]
 */
TEST(CircularBufferPrint, PrintAfterOneIsPut)
{
    expectedOutput = "Circular buffer content:\n<42>\n";
    CircularBuffer_Put(&buffer, 42);
    CircularBuffer_Print(&buffer);
    STRCMP_EQUAL(expectedOutput, actualOutput);
}

/**
 * CircularBufferPrint: PrintNotYetWrappedOrFull
 * - Change the if() checking that the buffer is empty by a for loop, starting
 *   in cb.read and stopping at cb.write, and call FormatOutput() with the
 *   buffer elemnt with that index. CB_Get() can't be used, because it changes
 *   the buffer state (advances read pointer)
 * - If pos=read, FormatOutput(element); in other case, FormatOutput(", element")
 */
TEST(CircularBufferPrint, PrintNotYetWrappedOrFull)
{
    expectedOutput = "Circular buffer content:\n<10, 20, 30>\n";
    CircularBuffer_Put(&buffer, 10);
    CircularBuffer_Put(&buffer, 20);
    CircularBuffer_Put(&buffer, 30);
    CircularBuffer_Print(&buffer);
    STRCMP_EQUAL(expectedOutput, actualOutput);
}

/**
 * CircularBufferPrint: PrintNotYetWrappedAndIsFull
 * This test should pass
 */
TEST(CircularBufferPrint, PrintNotYetWrappedAndIsFull)
{
    expectedOutput = "Circular buffer content:\n<31, 41, 59, 26, 53>\n";
    
    CircularBuffer cb = CircularBuffer_Create(5);
    CircularBuffer_Put(&cb, 31);
    CircularBuffer_Put(&cb, 41);
    CircularBuffer_Put(&cb, 59);
    CircularBuffer_Put(&cb, 26);
    CircularBuffer_Put(&cb, 53);

    CircularBuffer_Print(&cb);
    STRCMP_EQUAL(expectedOutput, actualOutput);

    CircularBuffer_Destroy(&cb);
}

/**
 * CircularBufferPrint: PrintOldToNewWhenWrappedAndFull
 * - In CB_PrintElements() define a local size_t print to replace cb.read and
 *   track printed items and initialize to cb.read
 * - Substitute the for loop for a while, looping while print != cb.write
 * - Call FormatOutput with argument cb.buf[print]
 * - Advance the print pointer using CB_NextIndex()
 */
TEST(CircularBufferPrint, PrintOldToNewWhenWrappedAndFull)
{
    int retval;
    expectedOutput = "Circular buffer content:\n<201, 202, 203, 204, 999>\n";
    
    CircularBuffer cb = CircularBuffer_Create(5);
    // Fill the buffer
    CircularBuffer_Put(&cb, 200);
    CircularBuffer_Put(&cb, 201);
    CircularBuffer_Put(&cb, 202);
    CircularBuffer_Put(&cb, 203);
    CircularBuffer_Put(&cb, 204);
    // Get the first item
    CircularBuffer_Get(&cb, &retval);
    // Put a new item
    CircularBuffer_Put(&cb, 999);

    CircularBuffer_Print(&cb);
    STRCMP_EQUAL(expectedOutput, actualOutput);

    CircularBuffer_Destroy(&cb);
}