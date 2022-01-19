#ifndef _FAKE_MICRO_TIME_H
#define _FAKE_MICRO_TIME_H


#include "MicroTime.h"

void FakeMicroTime_Init(uint32_t start, uint32_t increment);
uint32_t MicroTime_Get(void);

#endif