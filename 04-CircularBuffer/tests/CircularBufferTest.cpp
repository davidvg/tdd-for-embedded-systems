#include "CppUTest/TestHarness.h"

extern "C"
{
    #include "CircularBuffer.h"
    #include "RuntimeErrorStub.h"
}


TEST_GROUP(CircularBuffer)
{
    CircularBuffer buffer;
    size_t cap = 10;

    void setup()
    {
    buffer = CircularBuffer_Create(cap);
    }

    void teardown()
    {
        CircularBuffer_Destroy(&buffer);
    }

    void putManyInTheBuffer(int howMany, int offset)
    {
        for (int i=0; i<howMany; i++)
        {
            CircularBuffer_Put(&buffer, i+offset);
        }
    }
};

/**
 * CircularBuffer: EmptyAfterCreation
 * - CircularBuffer.h: Create a typedef struct CircularBuffer with an size_t size
 *   to avoid error
 * - CircularBuffer.h: Declare _Create(cb) and _Destroy(*cb)
 * - CircularBuffer.h: Declare CircularBuffer_IsEmpty(*cb)
 * - CircularBuffer.c: implement fucntions with empty bodies. IsEmpty() returns
 *   true.
 */
TEST(CircularBuffer, EmptyAfterCreation)
{
    CHECK_TRUE(CircularBuffer_IsEmpty(&buffer)); 
}

/**
 * CircularBuffer: NotFullAfterCreation
 * TODO Check if this test does anything
 * - CircularBuffer.c: define CircularBuffer_IsFull(*cb) and implement it to
 *   return false
 */
TEST(CircularBuffer, NotFullAfterCreation)
{
    CHECK_FALSE(CircularBuffer_IsFull(&buffer));
}

/**
 * CircularBuffer: NotEmptyAfterPut
 * - Declare CircularBuffer_Put(*cb, value)
 * - CircularBuffer.h: add int *buf and the two indices: int read and int write
 *   to the CircularBuffer struct
 * - In Create() initialize capacity to the capacity argument + 1 and both
 *   indices to 0
 * - In create(), allocate an array of ints b using malloc and make cb.buf = b
 *   and make cb->buf = b
 * - In CB_Put() set the element with index cb.write to the value and increment
 *   cb.write in 1
 * - In _IsEmpty() return cb.write == cb.read
 * - In Destroy() free the buffer
 */
TEST(CircularBuffer, NotEmptyAfterPut)
{
    CircularBuffer_Put(&buffer, 314);
    CHECK_FALSE(CircularBuffer_IsEmpty(&buffer));
}

/**
 * CircularBuffer: NotEmptyAfterPutThenEmptyAfterGet
 * TODO The previous test does the same than this one
 * - Declare CircularBuffer_Get(*cb)
 * - In _Get() return cb.buf[cb.read] and increment cb.read
 */
TEST(CircularBuffer, NotEmptyAfterPutThenEmptyAfterGet)
{
    CircularBuffer_Put(&buffer, 1234);
    CHECK_FALSE(CircularBuffer_IsEmpty(&buffer));
    CircularBuffer_Get(&buffer);
    CHECK_TRUE(CircularBuffer_IsEmpty(&buffer));
}

/**
 * CircularBuffer: GetReturnsPutValue
 * This test should pass
 */
TEST(CircularBuffer, GetReturnsPutValue)
{
    CircularBuffer_Put(&buffer, 42);
    LONGS_EQUAL(42, CircularBuffer_Get(&buffer));
}

/**
 * CircularBuffer: PutAndGetMultipleValues
 * This test should pass
 */
TEST(CircularBuffer, PutAndGetMultipleValues)
{
    putManyInTheBuffer(3, 100);
    LONGS_EQUAL(100, CircularBuffer_Get(&buffer));
    LONGS_EQUAL(101, CircularBuffer_Get(&buffer));
    LONGS_EQUAL(102, CircularBuffer_Get(&buffer));
}

/**
 * CircularBuffer: GetBufferCapacity
 * - Declare CircularBuffer_GetCapacity() and return cb.capacity-1 (the actual
 *   buffer size is capacity+1 to allow a complete full buffer)
 */
TEST(CircularBuffer, GetBufferCapacity)
{
    LONGS_EQUAL(cap, CircularBuffer_GetCapacity(&buffer));
}

/**
 * CircularBuffer: BufferIsFull
 * The buffer is considered FULL when there's only one free position for 
 * write in the buffer. This allows to distinguish between empty and full
 * buffers.
 * - Create a function CB_NextIndex(ix, cap) to compute next value for the index
 * - In _IsFull() return true when NextIndex(write, cap) = read
 */
TEST(CircularBuffer, BufferIsFull)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    putManyInTheBuffer(bufCapacity-1, 100);
    CHECK_FALSE(CircularBuffer_IsFull(&buffer));

    CircularBuffer_Put(&buffer, 999);
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));
}

/**
 * CircularBuffer: EmptyToFullToEmpty
 * This test should pass
 */
TEST(CircularBuffer, EmptyToFullToEmpty)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    putManyInTheBuffer(bufCapacity, 100);
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));

    for (int i=0; i<bufCapacity; i++)
    {
        LONGS_EQUAL(100+i, CircularBuffer_Get(&buffer));
    }
    CHECK_FALSE(CircularBuffer_IsFull(&buffer));
    CHECK_TRUE(CircularBuffer_IsEmpty(&buffer));
}

/**
 * CircularBuffer: PutWhenFullReturnsFalseAndThrowsRuntimeError
 * - Add a new error to the enum: BUFFER_IS_FULL
 * - Modify _Put() to return NOERROR on success, BUFFER_IS_FULL on failure, by
 *   checking using CircularBuffer_IsFull()
 * - Return a RUNTIME_ERROR
 */
TEST(CircularBuffer, PutWhenFullReturnsFalseAndThrowsRuntimeError)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    putManyInTheBuffer(bufCapacity, 100);
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));
    LONGS_EQUAL(BUFFER_IS_FULL, CircularBuffer_Put(&buffer, 999));
    STRCMP_EQUAL("Can't write to buffer. The buffer is full.", RuntimeErrorStub_GetLastError());
    LONGS_EQUAL(BUFFER_IS_FULL, RuntimeErrorStub_GetLastParameter());
}

/**
 * CircularBuffer: PutToFullDoesNothing
 * The test should pass
 */
TEST(CircularBuffer, PutToFullDoesNothing)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    putManyInTheBuffer(10 * bufCapacity, 1000);
    for (int i=0; i<bufCapacity; i++)
    {
        LONGS_EQUAL(1000 + i, CircularBuffer_Get(&buffer));
    }
}

/**
 * CircularBuffer, WrapIndices
 * - Modify _Put() and _Get() to update indices using NextIndex()
 */
TEST(CircularBuffer, WrapIndicesWhenPutting)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    // Fill the buffer. Result: cap=10+1, w=10, r=0
    putManyInTheBuffer(bufCapacity, 100);
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));
    // Get one item. Result: cap=10+1, w=10, r=1
    LONGS_EQUAL(100, CircularBuffer_Get(&buffer));
    CHECK_FALSE(CircularBuffer_IsFull(&buffer));
    //  Put a new item and check wrapping for write. Result: cap=10+1, w=0, r=1
    CircularBuffer_Put(&buffer, 200);
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));
}

/**
 * CircularBuffer: WrapIndicesWhenGetting
 * The test should pass
 */
TEST(CircularBuffer, WrapIndicesWhenGetting)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    // Fill the buffer
    putManyInTheBuffer(bufCapacity, 100);
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));
    // Leave only 1 item in the buffer
    for (int i=0; i<bufCapacity-1; i++)
    {
        LONGS_EQUAL(100+i, CircularBuffer_Get(&buffer));
    }
    // Put capacity/2 items in the buffer
    putManyInTheBuffer(bufCapacity/2, 100+(int)cap);
    // Get all the elements
    int j = 0;
    while(!CircularBuffer_IsEmpty(&buffer))
    {
        LONGS_EQUAL(100 + (int)cap - 1 + j, CircularBuffer_Get(&buffer));
        j++;
    }
    CHECK_TRUE(CircularBuffer_IsEmpty(&buffer));
}