extern "C"
{
    #include "LightScheduler.h"
    #include "LightControllerSpy.h"
    #include "FakeTimeService.h"
    #include "FakeRandomMinute.h"
}

#include "CppUTest/TestHarness.h"


TEST_GROUP(LightSchedulerRandomizeTest)
{
    void setup()
    {
        LightScheduler_Create();
        LightController_Create();
        UT_PTR_SET(RandomMinute_Get, FakeRandomMinute_Get); 
    }

    void teardown()
    {
        LightScheduler_Destroy();
        LightController_Destroy();
    }

    void setTimeTo(int day, int minute)
    {
        FakeTimeService_SetDay(day);
        FakeTimeService_SetMinute(minute);
    }

    void checkLightState(int id, int state)
    {
        if (id == LIGHT_ID_UNKNOWN)
        {
            // TODO Check if this is needed anymore 
            LONGS_EQUAL(id, LightControllerSpy_GetLastId());
            LONGS_EQUAL(state, LightControllerSpy_GetLastState());   
        }
        else
        {
            LONGS_EQUAL(state, LightControllerSpy_GetLightState(id));
        }
         
    }
};

/**
 * LightSchedulerRandomizeTest: TurnsOnEarly
 * - mocks/FakeRandomMinute.h: create.
 * - mocks/FakeRandomMinute.c: create.
 * - FakeRandomMinute.h: declare FakeRandomMinute_SetFirstAndIncrement().
 * - LightScheduler.h: declare LightScheduler_Randomize().
 * - LightSchedulerRandomizeTest.cpp: copy setTimeTo() and checkLightState()
 *   from lightSchedulerTest.cpp.
 * - FakeRandomMinute.c: implement FakeRandomMinute_SetFirstAndIncrement() to
 *   set two statics: seed and increment.
 * - LightScheduler.c: create LightScheduler_Randomize() with empty body.
 * - LightScheduler.c: add the int randomize and int randomMinutes to the struct.
 * - LightScheduler.c: add RANDOM_ON and RANDOM_OFF to the enum.
 * - LightScheduler.c: when calling Create(), ScheduleTurnOn() or
 *   ScheduleTurnOff(), set randomize = RANDOM_OFF and randomMinutes = 0.
 * - LightScheduler.c: implement LightScheduler_Randomize() to check if id, day
 *   and minute are the same for the current time and the event, and if so set
 *   randomize = RANDOM_ON and randomMinutes = RandomMinutes_Get().
 * - RandomMinute.h: change the declaration for RandomMinute_Get() to use a
 *   function pointer:
 *      extern int (*RandomMinute_Get) (void);
 * - RandomMinute.c: rename RandomMinute_Get() to RandomMinute_GetImpl().
 * - RandomMinute.c: assign the test double to RandomMinute_GetImpl() to
 *   implement RandomMinute_Get():
 *      int (*RandomMinute_Get)(void) = RandomMinute_GetImpl;
 * - LightSchedulerRandomizeTest.cpp: use the pointer macro for setting the
 *   funtion pointer in setup() and use FakeRandomMinute_Get() instead of
 *   RandomMinute_Get().
 * - FakeRandomMinute.h: declare FakeRandomMinute_Get().
 * - FakeRandomMinute.c: implement FakeRandomMinute_Get() to return seed and
 *   update seed to seed + increment.
 * - LightScheduler.c: Modify processEventNow() to compute
 *   event.minuteOfDay + event.randomMinutes and use this sum to check with
 *   time.minuteOfDay.
 * - LightScheduler-c: update WakeUp() 
 */
TEST(LightSchedulerRandomizeTest, TurnsOnEarly)
{
    FakeRandomMinute_SetFirstAndIncrement(-10, 5);
    LightScheduler_ScheduleTurnOn(4, EVERYDAY, 600);
    LightScheduler_Randomize(4, EVERYDAY, 600);
    setTimeTo(MONDAY, 600-10);
    LightScheduler_WakeUp();
    checkLightState(4, LIGHT_ON);
}
