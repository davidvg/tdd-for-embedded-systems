#include "FakeMicroTime.h"

static uint32_t time;
static uint32_t increment;

void FakeMicroTime_Init(uint32_t start, uint32_t increm)
{
    time = start;
    increment = increm;
}

uint32_t MicroTime_Get(void)
{
    time += increment;
    return time;
}