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
        LONGS_EQUAL(id, LightControllerSpy_GetLastId());
        LONGS_EQUAL(state, LightControllerSpy_GetLastState());
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