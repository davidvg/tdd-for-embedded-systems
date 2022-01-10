/**
 * CircularBuffer.h
 * 
 * Implementation of a Circular Buffer for integer values.
 */

#ifndef D_CIRCULAR_BUFFER_H
#define D_CIRCULAR_BUFFER_H

#include <stdbool.h>
#include "RuntimeError.h"
#include "RuntimeErrorStub.h"
#include "FormatOutput.h"

typedef enum
{
    NOERROR=0,
    BUFFER_IS_FULL,
    BUFFER_IS_EMPTY,
} CBError;

typedef struct
{
    size_t capacity;
    int *buf;
    size_t write;
    size_t read;
    CBError error;
} CircularBuffer;

CircularBuffer CircularBuffer_Create(size_t capacity);
void CircularBuffer_Destroy(CircularBuffer *cb);
bool CircularBuffer_IsEmpty(CircularBuffer *cb);
bool CircularBuffer_IsFull(CircularBuffer *cb);
CBError CircularBuffer_Put(CircularBuffer *cb, int val);
CBError CircularBuffer_Get(CircularBuffer *cb, int *retval);
size_t CircularBuffer_GetCapacity(CircularBuffer *cb);
void CircularBuffer_Print(CircularBuffer *cb);

#endif