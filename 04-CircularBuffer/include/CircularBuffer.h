/**
 * CircularBuffer.h
 * 
 * Implementation of a Circular Buffer for integer values.
 * 
 * TODO A guide for implementing full buffer using bool:
 * https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/
 *  
 */

#ifndef D_CIRCULAR_BUFFER_H
#define D_CIRCULAR_BUFFER_H

#include <stdbool.h>
#include "RuntimeError.h"
#include "RuntimeErrorStub.h"

typedef enum
{
    NOERROR=0,
    CAPACITY_NOT_POWER_OF_TWO,
    BUFFER_IS_FULL,
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
int CircularBuffer_Get(CircularBuffer *cb);
size_t CircularBuffer_GetCapacity(CircularBuffer *cb);

#endif