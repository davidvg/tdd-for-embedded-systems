#include "CircularBuffer.h"

#include <stdio.h>

static void CircularBuffer_Initialize(CircularBuffer *cb, size_t cap, CBError err)
{
    cb->capacity = cap;
    cb->write = 0;
    cb->read = 0;
    cb->error = err;

    size_t s = sizeof(int) * cb->capacity;
    int *b;
    b = (int *) malloc(s);
    cb->buf = b;
}

static bool CircularBuffer_CapacityIsPowerOfTwo(size_t cap)
{
    return (cap & (cap - 1)) == 0;
}

CircularBuffer CircularBuffer_Create(size_t capacity)
{
    CircularBuffer circBuff;

    if (CircularBuffer_CapacityIsPowerOfTwo(capacity))
    {
        CircularBuffer_Initialize(&circBuff, capacity, NOERROR);
    }
    else
    {
        RUNTIME_ERROR("Capacity is not a power of two", (int)capacity);
        CircularBuffer_Initialize(&circBuff, 0, CAPACITY_NOT_POWER_OF_TWO);
    }

    return circBuff;
}

void CircularBuffer_Destroy(CircularBuffer *cb)
{
    cb->capacity = 0;
    cb->write = cb->read = 0;
    cb->error = NOERROR;
    free(cb->buf);
}

bool CircularBuffer_IsEmpty(CircularBuffer *cb)
{
    return (cb->write == cb->read);
}

bool CircularBuffer_IsFull(CircularBuffer *cb)
{
    return (cb->write - cb->read == cb->capacity);
}

bool CircularBuffer_Put(CircularBuffer *cb, int val)
{
    if (CircularBuffer_IsFull(cb))
    {
        return false;
    }

    cb->buf[cb->write] = val;
    cb->write += 1;
    return true;
}

int CircularBuffer_Get(CircularBuffer *cb)
{
    int retval = cb->buf[cb->read];
    cb->read += 1;
    return retval;
}

size_t CircularBuffer_GetCapacity(CircularBuffer *cb)
{
    return cb->capacity;
}