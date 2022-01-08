#include "CircularBuffer.h"


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

size_t CircularBuffer_NextIndex(size_t index, size_t capacity)
{
    index++;
    if (index == capacity)
        index = 0;
    return index;
}

/**
 * The buffer is considered full when there's one free slot for write.
 * To have a full capacity, without losing one slot, the actual buffer size
 * is defined as capacity+1. This way the buffer will be full when 'capacity'
 * elements have been written.
 */
CircularBuffer CircularBuffer_Create(size_t capacity)
{
    CircularBuffer circBuff;
    CircularBuffer_Initialize(&circBuff, capacity+1, NOERROR);
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

/** 
 * The buffer is considered FULL when there is only one free slot for write.
 */
bool CircularBuffer_IsFull(CircularBuffer *cb)
{
    return (CircularBuffer_NextIndex(cb->write, cb->capacity) == cb->read);
}

CBError CircularBuffer_Put(CircularBuffer *cb, int val)
{
    if (CircularBuffer_IsFull(cb))
    {
        return BUFFER_IS_FULL;
    }

    cb->buf[cb->write] = val;
    cb->write += 1;
    return NOERROR;
}

int CircularBuffer_Get(CircularBuffer *cb)
{
    int retval = cb->buf[cb->read];
    cb->read += 1;
    return retval;
}

size_t CircularBuffer_GetCapacity(CircularBuffer *cb)
{
    return cb->capacity - 1;
}