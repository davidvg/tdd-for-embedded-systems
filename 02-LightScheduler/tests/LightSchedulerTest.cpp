extern "C"
{
    #include "LightScheduler.h"
    #include "LightControllerSpy.h"
    #include "FakeTimeService.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(LightScheduler)
{
    void setup()
    {
        LightScheduler_Create();
        LightController_Create();
    }

    void teardown()
    {
        LightScheduler_Destroy();
        LightController_Create();
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
 * LightScheduler: NoChangeToLightsDuringInitialization
 * - Create src/LightScheduler.h and declare the Create() and Destroy() functions.
 * - Create src/LightScheduler.c and implement Create() and Destroy() with empty
 *   bodies.
 * - Create include/LightController.h interface and declare the Create() and
 *   Destroy() functions.
 * - Create test/LightControllerSpy.h and declare LightControllerSpy_GetLastId()
 *   and LightControllerSpy_GetLastState(). Create the ENUM with the required
 *   elements to pass the test.
 * - Create LightControllerSpy.c and create two static int, lastId and lastState.
 *   In LightControllerSpy_Create(), initialize lastId and lastState.
 * - In LightControllerSpy.c implement LightControllerSpy_GetLastId() and
 *   LightControllerSpy_GetLastState() to return lastId and lastState.
 */
TEST(LightScheduler, NoChangeToLightsDuringInitialization)
{
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: NoScheduleNothingHappens
 * - In LightScheduler.h, declare the WakeUp function.
 * - In LightScheduler.c, implement the function with an empty skeleton to
 *   pass the test.
 */
TEST(LightScheduler, NoScheduleNothingHappens)
{
    setTimeTo(MONDAY, 100);

    LightScheduler_WakeUp();
    
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);

}

/**
 * LightScheduler: ScheduleOnEverydayNotTimeYet
 * - Add EVERYDAY to the enum in LightScheduler.h
 * - In LightScheduler.h, declare the ScheduleTurnOn() function. An empty
 *   implementation passes the test.
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
 * - In LightScheduler.c create a typedef struct ScheduledLightEvent to hold
 *   the scheduled event.
 * - Create a static ScheduledLightEvent to hold the last scheduled event.
 * - In LightScheduler.c, create an enum to hold UNUSED.
 * - Update scheduledEvent in the ScheduleTurnOn() function.
 * - In WakeUp(), get the time using TimeService_GetTime(); check if id is valid
 *   and if it's time to run the event; in that case, call LightController_On(id).
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
 * - Declare LightScheduler_ScheduleTurnOff() in the header file and implement
 *   it in the source file, to update lastId and lastState.
 * - Add new elements to the ENUM in LightScheduler.c, defining different types
 *   of events: TURN_ON, TURN_OFF.
 * - Add a new element to the ScheduledLightEvent: event. It will be one of
 *   TURN_ON, TURN_OFF.
 * - Update ScheduleTurnOn() and ScheduleTurnOff() to update the event attribute
 *   of ScheduledLightEvent.
 * - In WakeUp() check the value of scheduledEvent.event to decide which function
 *   to call.
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
 * - Add a day component to ScheduledLightEvent.
 * - In ScheduleTurnOn() and ScheduleTurnOff() update the value of day inside
 *   scheduledEvent.
 * - Inside processEvent() check that the event.day is not EVERYDAY to pass the
 *   test.
 */
TEST(LightScheduler, ScheduleTuesdayButItsMonday)
{
    LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
    setTimeTo(MONDAY, 1200);

    LightScheduler_WakeUp();

    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleTuesdayAndItsTuesday
 * - In processEventNow() check it's not EVERYDAY AND today is the actual
 *   day to react.
 */
TEST(LightScheduler, ScheduleTuesdayAndItsTuesday)
{
    LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
    setTimeTo(TUESDAY, 1200);

    LightScheduler_WakeUp();

    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleWeekEndItsFriday
 * - Add WEEKEND to the enum in LightScheduler.h
 */
TEST(LightScheduler, ScheduleWeekEndItsFriday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(FRIDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleWeekEndItsSaturday
 * - Refactor to create doesLightRespondToday(), a function that checks if light
 *   should respond today. It returns 1 on success and 0 on failure.
 * - Inside this function, return 1 when reactionDay=EVERYDAY, reactionDay=today
 *   or (reactionDay==WEEKEND && today==SATURDAY)
 */
TEST(LightScheduler, ScheduleWeekEndItsSaturday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(SATURDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleWeekEndItsSunday
 * - Modify the WEEKEND check in doesLightRespondToday() to apply when today is
 *   SATURDAY or SUNDAY
 */
TEST(LightScheduler, ScheduleWeekEndItsSunday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleWeekEndItsMonday
 * Passes.
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
 * - Add WEEKDAY to the enum
 */
TEST(LightScheduler, ScheduleWeekdayItsSunday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

/**
 * LightScheduler: ScheduleWeekdayItsMonday
 * - Check that reactionDay=WEEKDAY and today=MONDAY
 */
TEST(LightScheduler, ScheduleWeekdayItsMonday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: ScheduleWeekdayItsFriday
 * - Modify the WEEKDAY check to include today>=MONDAY and today<=FRIDAY
 */
TEST(LightScheduler, ScheduleWeekdayItsFriday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(FRIDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

/**
 * LightScheduler: RememberAllLightStates
 * - Declare LightControllerSpy_GetLightState(int) in LightControllerSpy.h
 * - Implement it in LightContollerSpy.c with empty body.
 * - Modify checkLightState to check if id=UNKNOWN; in this case, do as before;
 *   in other case, call GetLightState(id)
 * - Inside LightScheduler.c add MAX_EVENTS = 128 to the enum.
 * - Inside LightScheduler.c create an array of ScheduledLightEvents 
 * - In LightScheduler_Create() add a foor loop to initialize every id event in
 *   scheduledEvents to UNUSED
 * - Add a for loop in scheduleEvent() to find first event with id=UNUSED and 
 *   update its values. Update scheduledEvent (the variable) as before.
 * - In WakeUp() add a for loop to process every event. Keep the processEvent for
 *   the scheduledEvent variable.
 * - In LightController.h define MAX_LIGHTS = 32 (32 lights, 128 possible events)
 * - In LightControllerSpy.c create an array lights[MAX_LIGHTS].
 * - In LightControllerSpy_Create() initalize all lights to LIGHT_STATE_UNKNOWN.
 * - Update LightController_ON() and _Off() to update lights[id] with the state.
 * - Make GetLightState return the state for lights[id]
 */
TEST(LightScheduler, RememberAllLightStates)
{
    LightController_On(0);
    LightController_On(31);
    LONGS_EQUAL(LIGHT_ON, LightControllerSpy_GetLightState(0));
    LONGS_EQUAL(LIGHT_ON, LightControllerSpy_GetLightState(31));
}

/**
 * LightScheduler: ScheduleTwoEventsAtTheSameTime
 * This test should pass when the previous one passes
 */
TEST(LightScheduler, ScheduleTwoEventsAtTheSameTime)
{
    LightScheduler_ScheduleTurnOn(3, SUNDAY, 1200);
    LightScheduler_ScheduleTurnOn(12, SUNDAY, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
    checkLightState(12, LIGHT_ON);
}

/******************************************************************************/
/******************* LIGHT SCHEDULER INIT AND CLEANUP TESTS *******************/
/******************************************************************************/

TEST_GROUP(LightSchedulerInitAndCleanup)
{

};

/**
 * LightSchedulerInitAndCleanup: CreateStartsOneMinuteAlarm
 * FakeTimeService_GetAlarmCallback() returns the scheduled callback for an
 * event. When the Scheduler is created, this callback is defaulted to call
 * WakeUp every 60 seconds.
 * FakeTimeService spies on the call to this function.
 * - In FakeTimeService.h declare FakeTimeService_GetAlarmCallback() that returns
 *   a WakeUpCallback object. This type is defined in TimeService.h as a pointer
 *   to a function, returning void.
 * - In FakeTimeService.h declare FakeTimeService_GetAlarmPeriod() and
 *   FakeTimeService_GetAlarmCallback()
 * - Implement both with empty bodies.
 * 
 * Now we set a callback to be called by LightScheduler every 60 seconds.
 * - To add a callback, a function must be created in TimeService.h, called
 *   TimeService_SetPeriodicAlarmInSeconds().
 * - This function is implemented in FakeTimeService.c to spy on the original.
 *   It updates two static variables: period and callback.
 * - In LightScheduler_Create(), call TimeService_SetPeriodicAlarmInSeconds()
 *   calling LightScheduler_WakeUp() every 60 seconds.
 * 
 * We need to obtain the callback from FakeTimeService, implementing the
 * previously GetAlarmCallback() and GetAlarmPeriod() functions.
 * - Make the functions return the period and the callback, respectively.
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
 * - In LightScheduler_Destroy() call TimeService_CancelPeriodicAlarmInSeconds,
 *   with args 60 and LightScheduler_WakeUp;
 * - Declare this function in the interface for TimeService.h
 * - Implement it in FakeTimeService.c, checking that period and callback are
 *   equal to the current values and setting period=0 and callback=NULL
 */
TEST(LightSchedulerInitAndCleanup, DestroyCancelsOneMinuteAlarm)
{
    LightScheduler_Create();
    LightScheduler_Destroy();
    POINTERS_EQUAL(NULL, (void *)FakeTimeService_GetAlarmCallback());
}