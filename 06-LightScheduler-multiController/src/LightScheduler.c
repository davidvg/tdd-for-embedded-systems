#include "LightScheduler.h"
#include "TimeService.h"
#include "LightController.h"
#include "RandomMinute.h"

enum {
    UNUSED = -1,
    TURN_ON = 0,
    TURN_OFF,
    RANDOM_ON,
    RANDOM_OFF,
    MAX_EVENTS = 128,
};

typedef struct {
    int id;
    Day day;
    int minuteOfDay;
    int event;
    int randomize;
    int randomMinutes;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvents[MAX_EVENTS];

static void operateLight(ScheduledLightEvent * lightEvent)
{
    if (lightEvent->event == TURN_ON)
        LightController_TurnOn(lightEvent->id);
    else if (lightEvent->event == TURN_OFF)
        LightController_TurnOff(lightEvent->id);
}

static int doesLightRespondToday(Time *time, int reactionDay)
{
    int today = time->dayOfWeek;

    if (reactionDay == EVERYDAY)
        return TRUE;
    if (reactionDay == today)
        return TRUE;
    if (reactionDay == WEEKEND && (today == SATURDAY || today == SUNDAY))
        return TRUE;
    if (reactionDay == WEEKDAY && today >= MONDAY && today <= FRIDAY)
        return TRUE;

    return FALSE;
}

static void processEventNow(Time *time, ScheduledLightEvent *lightEvent)
{
    int modifiedMinute = lightEvent->minuteOfDay + lightEvent->randomMinutes;

    if (lightEvent->id == UNUSED)
        return;
    if (modifiedMinute != time->minuteOfDay)
        return;
    if (!doesLightRespondToday(time, lightEvent->day))
        return;

    operateLight(lightEvent);
}

static int scheduleEvent(int id, Day day, int minute, int event)
{
    if (id < 0 || id > MAX_LIGHTS-1)    // id goes from 0 to MAX_LIGHTS-1
        return LS_ID_OUT_OF_BOUNDS;

    for (int i=0; i<MAX_EVENTS; i++)
    {
        if (scheduledEvents[i].id == UNUSED)
        {
            scheduledEvents[i].id = id;
            scheduledEvents[i].day = day;
            scheduledEvents[i].minuteOfDay = minute;
            scheduledEvents[i].event = event;
            scheduledEvents[i].randomize = RANDOM_OFF;
            scheduledEvents[i].randomMinutes = 0;
            return LS_OK;
        }
    }
    return LS_TOO_MANY_EVENTS;
}

void LightScheduler_Create(void)
{
    for (int i=0; i<MAX_EVENTS; i++)
    {
        scheduledEvents[i].id = UNUSED;
    }

    TimeService_SetPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

void LightScheduler_Destroy(void)
{
    TimeService_CancelPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

void LightScheduler_WakeUp(void)
{
    Time time;
    TimeService_GetTime(&time);

    for (int i=0; i<MAX_EVENTS; i++)
    {
        processEventNow(&time, &scheduledEvents[i]);
    }
}

int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
    return scheduleEvent(id, day, minuteOfDay, TURN_ON);
}

int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{

    return scheduleEvent(id, day, minuteOfDay, TURN_OFF);
}

void LightScheduler_ScheduleRemove(int id, Day day, int minuteOfDay)
{
    for (int i=0; i<MAX_EVENTS; i++)
    {
        if (scheduledEvents[i].id == id
            && scheduledEvents[i].day == day
            && scheduledEvents[i].minuteOfDay == minuteOfDay)
        {
            scheduledEvents[i].id = UNUSED;
        }
    }
}

void LightScheduler_Randomize(int id, Day day, int minuteOfDay)
{
    for (int i=0; i<MAX_EVENTS; i++)
    {
        ScheduledLightEvent *event = &scheduledEvents[i];
        if (event->id == id &&
            event->day == day &&
            event->minuteOfDay == minuteOfDay)
        {
            event->randomize = RANDOM_ON;
            event->randomMinutes = RandomMinute_Get();
        }
        // break;  // TODO check this!!
    }
}