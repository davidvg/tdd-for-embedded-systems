#include "CppUTest/TestHarness.h"

extern "C"
{
    #include "CircularBuffer.h"
}


TEST_GROUP(CircularBuffer)
{
    CircularBuffer buffer;
    size_t capacity = 8;

    void setup()
    {
    buffer = CircularBuffer_Create(capacity);
    }

    void teardown()
    {
        CircularBuffer_Destroy(&buffer);
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
 * CircularBuffer: CapacityIsPowerOfTwoReturnsErrorCode
 * - CircularBuffer.h: create enum with error codes
 * - In the struct add a CBError error field
 * - Create static function to check capacity = 2^n
 * - In Create() check if is power of two and initilize with error and
 *   capacity=0, to create a buffer with size 0
 * - Add a RUNTIME_ERROR when capacity is wrong
 */
TEST(CircularBuffer, CapacityNotPowerOfTwoReturnsErrorCode)
{
    CircularBuffer b = CircularBuffer_Create(10);
    LONGS_EQUAL(CAPACITY_NOT_POWER_OF_TWO, b.error);
    STRCMP_EQUAL("Capacity is not a power of two", RuntimeErrorStub_GetLastError());
    LONGS_EQUAL(10, RuntimeErrorStub_GetLastParameter());
    CircularBuffer_Destroy(&b);
}

/**
 * CircularBuffer: CapacityIsPowerOfTwoReturnsNoError
 */
TEST(CircularBuffer, CapacityIsPowerOfTwoReturnsNoError)
{
    LONGS_EQUAL(NOERROR, buffer.error);
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
 * - In Create() initialize size to the size argument and both indices to 0
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
    CircularBuffer_Put(&buffer, 101);
    CircularBuffer_Put(&buffer, 102);
    CircularBuffer_Put(&buffer, 103);
    LONGS_EQUAL(101, CircularBuffer_Get(&buffer));
    LONGS_EQUAL(102, CircularBuffer_Get(&buffer));
    LONGS_EQUAL(103, CircularBuffer_Get(&buffer));
}

/**
 * CircularBuffer: GetBufferCapacity
 * - Declare CircularBuffer_GetCapacity() and return cb.capacity
 */
TEST(CircularBuffer, GetBufferCapacity)
{
    LONGS_EQUAL(capacity, CircularBuffer_GetCapacity(&buffer));
}

/**
 * CircularBuffer: BufferIsFull
 * The buffer is considered FULL when there's only one free position for 
 * write in the buffer. This allows to distinguish between empty and full
 * buffers.
 * - In _IsFull() return true when write-read = capacity-1
 */
TEST(CircularBuffer, BufferIsFull)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    for (int i=0; i<bufCapacity-2; i++)
    {
        CircularBuffer_Put(&buffer, 100+i);
    }
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
    for (int i=0; i<bufCapacity-1; i++)
    {
        CircularBuffer_Put(&buffer, 100+i);
    }
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));

    for (int i=0; i<bufCapacity-1; i++)
    {
        LONGS_EQUAL(100+i, CircularBuffer_Get(&buffer));
    }
    CHECK_FALSE(CircularBuffer_IsFull(&buffer));
    CHECK_TRUE(CircularBuffer_IsEmpty(&buffer));
}

/**
 * CircularBuffer: PutWhenFullReturnsFalse
 * - Modify _Put() to return true on success, false on failure, by checking
 *   using CircularBuffer_IsFull()
 */
TEST(CircularBuffer, PutWhenFullReturnsFalse)
{
    int bufCapacity = (int)CircularBuffer_GetCapacity(&buffer);
    for (int i=0; i<bufCapacity-1; i++)
    {
        CircularBuffer_Put(&buffer, 100+i);
    }
    CHECK_TRUE(CircularBuffer_IsFull(&buffer));
    CHECK_FALSE(CircularBuffer_Put(&buffer, 999));
}