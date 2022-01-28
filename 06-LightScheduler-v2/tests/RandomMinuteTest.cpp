#include "CppUTest/TestHarness.h"
#include <cstring>

extern "C"
{
    #include "RandomMinute.h"
}

enum
{
    BOUND = 30,
};

TEST_GROUP(RandomMinute)
{
    int minute;

    void setup()
    {
        RandomMinute_Create(BOUND);
        srand(1);
    }

    void teardown()
    {

    }

    void AssertMinuteIsInRange()
    {
        if (minute < -BOUND || minute > BOUND)
        {
            printf("Bad minute value: %d\n", minute);
            FAIL("Minute out of range");
        }
    }
};

/**
 * RandomMinute: GetIsInRange
 * - Create RandomMinute.h and RandomMinute.c
 * - Create RandomMinute_Create() to initialize the bound stored in a static.
 * - Create RandomMinute_Get() to return a fixed value.
 */
TEST(RandomMinute, GetIsInRange)
{
    for (int i=0; i<1; i++)
    {
        minute = RandomMinute_Get();
        AssertMinuteIsInRange();
    }
}

/**
 * RandomMinute: AllValuesPossible
 * rand() generates ints with the size for the machine (32-bit on my PC)
 * To be sure that computations are OK, bound and minute must be the same type.
 * - Modify RandomMinute_Get() to return a value in range
 */
TEST(RandomMinute, AllValuesPossible)
{
    int hit[2*BOUND + 1];
    memset(hit, 0, sizeof(hit));

    for (int i=0; i<500; i++)
    {
        minute = RandomMinute_Get();
        AssertMinuteIsInRange();
        hit[minute + BOUND]++;
    }

    for (int i=0; i< 2*BOUND+1; i++)
    {
        CHECK(hit[i] > 0);
    }
}