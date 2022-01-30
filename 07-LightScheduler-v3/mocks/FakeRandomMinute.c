#include "FakeRandomMinute.h"

static int seed;
static int increment;

void FakeRandomMinute_SetFirstAndIncrement(int start, int incr)
{
    seed = start;
    increment = incr;
}

int FakeRandomMinute_Get(void)
{
    int result = seed;
    seed += increment;
    return result;
}