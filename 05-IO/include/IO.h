#ifndef _IO_H
#define _IO_H
#include <stdint.h>


typedef int ioAddress;
typedef int ioData;

void IO_Write(ioAddress addr, ioData data);
ioData IO_Read(ioAddress addr);


#endif