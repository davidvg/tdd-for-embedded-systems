extern "C"
{
    #include "LightScheduler.h"
    #include "LightController.h"
    #include "LightDriverSpy.h"
    #include "FakeTimeService.h"
}

#include "CppUTest/TestHarness.h"

/**
 */
TEST_GROUP(LightScheduler)
{
    void setup()
    {
        LightDriverSpy_Reset();
        LightDriverSpy_InstallInterface();

        LightController_Create();
		LightDriverSpy_AddSpiesToController();
        
        LightScheduler_Create();
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
 * LightScheduler: createDestroy
 * This test should pass
 */
TEST(LightScheduler, createDestroy)
{

}

/**
 * LightScheduler: NoChangeToLightsDuringInitialization
 * 
 */
TEST(LightScheduler, NoChangeToLightsDuringInitialization)
{
    LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightDriverSpy_GetLastId());
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetLastState());
}

/**
 * LightScheduler: NoScheduleNothingHappens
 */
TEST(LightScheduler, NoScheduleNothingHappens)
{
    setTimeTo(MONDAY, 100);

    LightScheduler_WakeUp();
    
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);

}

/**
 * LightScheduler: ScheduleOnEverydayNotTimeYet
 */
TEST(LightScheduler, ScheduleOnEverydayNotTimeYet)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1199);

    LightScheduler_WakeUp();
    
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleOnEverydayItsTime
 */
TEST(LightScheduler, ScheduleOnEverydayItsTime)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1200);

    LightScheduler_WakeUp();

    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleOffEverydayItsTime
 */
TEST(LightScheduler, ScheduleOffEverydayItsTime)
{
    LightScheduler_ScheduleTurnOff(3, EVERYDAY, 1200);
    FakeTimeService_SetDay(MONDAY);
    setTimeTo(MONDAY, 1200);

    LightScheduler_WakeUp();

    checkLightState(3, LIGHT_OFF);
}

/**
 * LightScheduler: ScheduleTuesdayButItsMonday
 */
TEST(LightScheduler, ScheduleTuesdayButItsMonday)
{
    LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
    setTimeTo(MONDAY, 1200);

    LightScheduler_WakeUp();

    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleTuesdayAndItsTuesdayAndItsTime
 */
TEST(LightScheduler, ScheduleTuesdayAndItsTuesdayAndItsTime)
{
    LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
    setTimeTo(TUESDAY, 1200);

    LightScheduler_WakeUp();

    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleOffTuesdayAndItsTuesdayAndItsTime
 */
TEST(LightScheduler, ScheduleOffTuesdayAndItsTuesdayAndItsTime)
{
    LightScheduler_ScheduleTurnOff(3, TUESDAY, 1200);
    setTimeTo(TUESDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_OFF);
}

/**
 * LightScheduler: ScheduleWeekEndItsFriday
 */
TEST(LightScheduler, ScheduleWeekEndItsFriday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(FRIDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleWeekEndItsSaturdayAndItsTime
 */
TEST(LightScheduler, ScheduleWeekEndItsSaturdayAndItsTime)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(SATURDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleWeekEndItsSundayAndItsTime
 */
TEST(LightScheduler, ScheduleWeekEndItsSundayAndItsTime)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleWeekEndItsMonday
 */
TEST(LightScheduler, ScheduleWeekEndItsMonday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleWeekdayItsSunday
 */
TEST(LightScheduler, ScheduleWeekdayItsSunday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleWeekdayItsMondayAndItsTime
 */
TEST(LightScheduler, ScheduleWeekdayItsMondayAndItsTime)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleWeekdayItsFridayAndItsTime
 */
TEST(LightScheduler, ScheduleWeekdayItsFridayAndItsTime)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(FRIDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: RememberAllLightStates
 */
TEST(LightScheduler, RememberAllLightStates)
{
    LightController_TurnOn(0);
    LightController_TurnOn(31);
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(0));
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(31));
}

/**
 * LightScheduler: ScheduleTwoEventsAtTheSameTime
 */
TEST(LightScheduler, ScheduleTwoEventsAtTheSameTime)
{
    LightScheduler_ScheduleTurnOn(3, SUNDAY, 1200);
    LightScheduler_ScheduleTurnOff(12, SUNDAY, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
    checkLightState(12, LIGHT_OFF);
}

/**
 * LightScheduler: ScheduleTwoEventsAtDifferentTime
 */
TEST(LightScheduler, ScheduleTwoEventsAtDifferentTime)
{
    LightScheduler_ScheduleTurnOff(4, MONDAY, 900);
    LightScheduler_ScheduleTurnOn(3, MONDAY, 900+1);

    setTimeTo(MONDAY, 900);
    LightScheduler_WakeUp();

    checkLightState(4, LIGHT_OFF);

    setTimeTo(MONDAY, 900+1);
    LightScheduler_WakeUp();

    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: RejectsTooManyEvents
 */
TEST(LightScheduler, RejectsTooManyEvents)
{
    for (int i=0; i<128; i++)
    {
        LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(6, MONDAY, 600+i));
    }
    LONGS_EQUAL(LS_TOO_MANY_EVENTS, LightScheduler_ScheduleTurnOn(6, MONDAY, 750));
}

/**
 * LightScheduler: RemoveScheduledEvent
 */
TEST(LightScheduler, RemoveScheduledEvent)
{
    LightScheduler_ScheduleTurnOn(6, MONDAY, 1200);
    LightScheduler_ScheduleRemove(6, MONDAY, 1200);

	setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();

	checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: RemoveRecyclesScheduleSlot
 */
TEST(LightScheduler, RemoveRecyclesScheduleSlot)
{
    for (int i=0; i<128; i++)
    {
        LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(6, MONDAY, 600+i));
    }
    LightScheduler_ScheduleRemove(6, MONDAY, 600);
    LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(6, MONDAY, 1000));
}

/**
 * LightScheduler: RemoveMultipleScheduledEvent
 */
TEST(LightScheduler, RemoveMultipleScheduledEvent)
{
    LightScheduler_ScheduleTurnOn(6, MONDAY, 600);
    LightScheduler_ScheduleTurnOn(7, MONDAY, 600);
    LightScheduler_ScheduleRemove(6, MONDAY, 600);

    setTimeTo(MONDAY, 600);
    LightScheduler_WakeUp();

    checkLightState(6, LIGHT_STATE_UNKNOWN);
    checkLightState(7, LIGHT_ON);
}

/**
 * LightScheduler: AcceptsValidLightIds
 */
TEST(LightScheduler, AcceptsValidLightIds)
{
    LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(0, MONDAY, 600));
    LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(15, MONDAY, 600));
    LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(31, MONDAY, 600));
}

/**
 * LightScheduler: RejectsInvalidLights
 */
TEST(LightScheduler, RejectsInvalidLights)
{
    LONGS_EQUAL(LS_ID_OUT_OF_BOUNDS,
                LightScheduler_ScheduleTurnOn(-1, MONDAY, 600));
    LONGS_EQUAL(LS_ID_OUT_OF_BOUNDS,
                LightScheduler_ScheduleTurnOn(32, MONDAY, 600));
}


/******************************************************************************/
/******************* LIGHT SCHEDULER INIT AND CLEANUP TESTS *******************/
/******************************************************************************/

TEST_GROUP(LightSchedulerInitAndCleanup)
{

};

/**
 * LightSchedulerInitAndCleanup: CreateStartsOneMinuteAlarm
 */
TEST(LightSchedulerInitAndCleanup, CreateStartsOneMinuteAlarm)
{
    LightScheduler_Create();
    POINTERS_EQUAL((void *)LightScheduler_WakeUp,
                   (void *)FakeTimeService_GetAlarmCallback());

    LONGS_EQUAL(60, FakeTimeService_GetAlarmPeriod());
    LightScheduler_Destroy();
}

/**
 * LightSchedulerInitAndCleanup: DestroyCancelsOneMinuteAlarm
 */
TEST(LightSchedulerInitAndCleanup, DestroyCancelsOneMinuteAlarm)
{
    LightScheduler_Create();
    LightScheduler_Destroy();
    POINTERS_EQUAL(NULL, (void *)FakeTimeService_GetAlarmCallback());
}