/**
 * CircularBuffer.h
 * 
 * Implementation of a Circular Buffer for integer values.
 *  
 */

#ifndef D_CIRCULAR_BUFFER_H
#define D_CIRCULAR_BUFFER_H

#include <stdbool.h>

typedef struct
{
    size_t capacity;
    int *buf;
    int write;
    int read;
} CircularBuffer;

CircularBuffer CircularBuffer_Create(size_t capacity);
void CircularBuffer_Destroy(CircularBuffer *cb);
bool CircularBuffer_IsEmpty(CircularBuffer *cb);
bool CircularBuffer_IsFull(CircularBuffer *cb);
void CircularBuffer_Put(CircularBuffer *cb, int val);
int CircularBuffer_Get(CircularBuffer *cb);
size_t CircularBuffer_GetCapacity(CircularBuffer *cb);

#endif