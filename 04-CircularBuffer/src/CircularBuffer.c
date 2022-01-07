#include "CircularBuffer.h"


CircularBuffer CircularBuffer_Create(size_t capacity)
{
    CircularBuffer circBuff;
    circBuff.capacity = capacity;    
    circBuff.write = 0;
    circBuff.read = 0;

    size_t s = sizeof(int) * capacity;
    int *b;
    b = (int *) malloc(s);
    circBuff.buf = b;
    
    return circBuff;
}

void CircularBuffer_Destroy(CircularBuffer *cb)
{
    cb->capacity = 0;
    cb->write = cb->read = 0;
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