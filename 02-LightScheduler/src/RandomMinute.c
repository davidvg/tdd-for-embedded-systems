#include "RandomMinute.h"
#include "stdio.h"

static int bound = 0;

void RandomMinute_Create(int b)
{
    bound = b;
}

int RandomMinute_Get(void)
{
    // rand() returns an int with the size for the machine (32-bit)
    int result = bound - (rand() % (2*bound+1));
    return result;
}