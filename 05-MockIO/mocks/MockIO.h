#ifndef _MOCK_IO_H
#define _MOCK_IO_H

#include "IO.h"

void MockIO_Create(int maxExpectations);
void MockIO_Destroy(void);
void MockIO_Expect_Write(ioAddress addr, ioData data);
void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData data);

#endif