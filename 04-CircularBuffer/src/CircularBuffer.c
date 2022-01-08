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

CircularBuffer CircularBuffer_Create(size_t capacity)
{
    CircularBuffer circBuff;
    CircularBuffer_Initialize(&circBuff, capacity, NOERROR);
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
    return (cb->write - cb->read == cb->capacity - 1);
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
    return cb->capacity;
}