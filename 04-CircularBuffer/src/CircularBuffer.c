#include "CircularBuffer.h"

/**
 * The buffer is considered full when there's one free slot for write.
 * To have a full capacity, without losing one slot, the actual buffer size
 * is defined as capacity+1. This way the buffer will be full when 'capacity'
 * elements have been written.
 */
static void CircularBuffer_Initialize(CircularBuffer *cb, size_t cap, CBError err)
{
    cb->capacity = cap + 1;
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
    index += 1;
    if (index == capacity)
        index = 0;
    return index;
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
        RUNTIME_ERROR("Can't write to buffer. The buffer is full.", BUFFER_IS_FULL);
        return BUFFER_IS_FULL;
    }

    cb->buf[cb->write] = val;
    cb->write = CircularBuffer_NextIndex(cb->write, cb->capacity);
    return NOERROR;
}

CBError CircularBuffer_Get(CircularBuffer *cb, int *retval)
{
    if(CircularBuffer_IsEmpty(cb))
    {
        RUNTIME_ERROR("Can't read from buffer. The buffer is empty.", BUFFER_IS_EMPTY);
        return BUFFER_IS_EMPTY;
    }
    *retval = cb->buf[cb->read];
    cb->read = CircularBuffer_NextIndex(cb->read, cb->capacity);
    return NOERROR;
}

size_t CircularBuffer_GetCapacity(CircularBuffer *cb)
{
    return cb->capacity - 1;
}

static void CircularBuffer_PrintElements(CircularBuffer *cb)
{
    for (size_t pos=cb->read; pos<cb->write; pos++)
    {
        if (pos == cb->read)
        {
            FormatOutput("%d", cb->buf[pos]);
        }
        else{
            FormatOutput(", %d", cb->buf[pos]);
        }
    }
}

void CircularBuffer_Print(CircularBuffer *cb)
{
    char * outputHead = "Circular buffer content:\n<";
    char * outputTail = ">\n";

    FormatOutput("%s", outputHead);

    CircularBuffer_PrintElements(cb);

    FormatOutput("%s", outputTail);
}