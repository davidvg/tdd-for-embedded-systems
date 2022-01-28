extern "C"
{
    #include "FakeTimeService.h"
    #include "LightScheduler.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(FakeTimeService)
{
    void setup()
    {
        TimeService_Create();
    }

    void teardown()
    {
        TimeService_Destroy();
    }
};

/**
 * FakeTimeService: Create
 * - Create the INTERFACE, include/TimeService.h
 * - Create tests/FakeTimeService.h and include include/TimeService.h
 * - Declare the interface: TimeService_Create(), TimeService_Destroy() and
 *   TimeService_GetTime(&t)
 * - In TimeService.h, create a typedef struct Time with the structure of the 
 *   Time data type.
 * - In FakeTimeService.h create an enum with TIME_UNKNOWN.
 * - Create tests/FakeTimeService.c and implement the interface. Create a
 *   static Time fakeTime to hold the fake time. Initialize this variable in
 *   the Create() function.
 */
TEST(FakeTimeService, Create)
{
    Time time;
    TimeService_GetTime(&time);
    LONGS_EQUAL(TIME_UNKNOWN, time.minuteOfDay);
    LONGS_EQUAL(TIME_UNKNOWN, time.dayOfWeek);
}

/**
 * FakeTimeService: Set
 * - Declare SetMinute() and SetDay() in FakeTimeService.h and implement them
 *   in FakeTimeService.c
 * - In LightScheduler.h create an enum with the days of the week
 */
TEST(FakeTimeService, Set)
{
    Time time;
    FakeTimeService_SetMinute(42);
    FakeTimeService_SetDay(SATURDAY);
    TimeService_GetTime(&time);
    LONGS_EQUAL(42, time.minuteOfDay);
    LONGS_EQUAL(SATURDAY, time.dayOfWeek);
}