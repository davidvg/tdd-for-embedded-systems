#include "RandomMinute.h"
#include "stdio.h"

static int bound = 0;

void RandomMinute_Create(int b)
{
    bound = b;
    // srand(1);
}

int RandomMinute_GetImpl(void)
{
    // rand() returns an int with the size for the machine (32-bit)
    int result = bound - (rand() % (2*bound+1));
    return result;
}

/**
 * In production t he function pointer is pointed to the actual function in the
 * source file. This can be overriden in tests by pointing to another function
 * that acts as a double for the actual one.
 * After the test is done the pointer should be restored to the production
 * function.
 */
int (*RandomMinute_Get)(void) = RandomMinute_GetImpl;