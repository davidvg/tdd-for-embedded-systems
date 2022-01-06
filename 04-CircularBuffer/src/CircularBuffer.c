#include "CircularBuffer.h"


CircularBuffer CircularBuffer_Create(size_t size)
{
    CircularBuffer circBuff;
    circBuff.size = size;    
    circBuff.write = 0;
    circBuff.read = 0;

    size_t s = sizeof(int) * size;
    int *b;
    b = (int *) malloc(s);
    circBuff.buf = b;
    
    return circBuff;
}

void CircularBuffer_Destroy(CircularBuffer *cb)
{
    cb->size = 0;
    cb->write = cb->read = 0;
    free(cb->buf);
}

bool CircularBuffer_IsEmpty(CircularBuffer *cb)
{
    return (cb->write == cb->read);
}

bool CircularBuffer_IsFull(CircularBuffer *cb)
{
    return false;
}

void CircularBuffer_Put(CircularBuffer *cb, int val)
{
    cb->buf[cb->write] = val;
    cb->write += 1;
}

int CircularBuffer_Get(CircularBuffer *cb)
{
    int retval = cb->buf[cb->read];
    cb->read += 1;
    return retval;
}