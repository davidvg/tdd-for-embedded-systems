extern "C"
{
    #include "LightScheduler.h"
    #include "LightDriverSpy.h"
    #include "FakeTimeService.h"
    #include "FakeRandomMinute.h"
}

#include "CppUTest/TestHarness.h"

/**
 * Test Group
 */
TEST_GROUP(LightSchedulerRandomizeTest)
{
    void setup()
    {
        LightController_Create();
        LightDriverSpy_Reset();
        LightDriverSpy_AddSpiesToController();
        LightScheduler_Create();

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
            LONGS_EQUAL(state, LightDriverSpy_GetLastState());   
        }
        else
        {
            LONGS_EQUAL(state, LightDriverSpy_GetState(id));
        }
         
    }
};

/**
 * LightSchedulerRandomizeTest: TurnsOnEarly 
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
