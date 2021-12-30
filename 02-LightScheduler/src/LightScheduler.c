#include "LightScheduler.h"
#include "TimeService.h"
#include "LightController.h"

enum {
    UNUSED = -1,
    TURN_ON = 0,
    TURN_OFF,
    MAX_EVENTS = 128,
};

typedef struct {
    int id;
    Day day;
    int minuteOfDay;
    int event;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvent;
static ScheduledLightEvent scheduledEvents[MAX_EVENTS];

static void operateLight(ScheduledLightEvent * lightEvent)
{
    if (lightEvent->event == TURN_ON)
        LightController_On(lightEvent->id);
    else if (lightEvent->event == TURN_OFF)
        LightController_Off(lightEvent->id);
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
    if (lightEvent->id == UNUSED)
        return;
    if (lightEvent->minuteOfDay != time->minuteOfDay)
        return;
    if (!doesLightRespondToday(time, lightEvent->day))
        return;

    operateLight(lightEvent);
}

static void scheduleEvent(int id, Day day, int minute, int event)
{
    for (int i=0; i<MAX_EVENTS; i++)
    {
        if (scheduledEvents[i].id == UNUSED)
        {
            scheduledEvents[i].id = id;
            scheduledEvents[i].day = day;
            scheduledEvents[i].minuteOfDay = minute;
            scheduledEvents[i].event = event;
            break;
        }
    }
    scheduledEvent.id = id;
    scheduledEvent.day = day;
    scheduledEvent.minuteOfDay = minute;
    scheduledEvent.event = event;
}

void LightScheduler_Create(void)
{
    scheduledEvent.id = UNUSED;

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

void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
    scheduleEvent(id, day, minuteOfDay, TURN_ON);
}

void LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
    scheduleEvent(id, day, minuteOfDay, TURN_OFF);
}

void LightScheduler_WakeUp(void)
{
    Time time;
    TimeService_GetTime(&time);

    for (int i=0; i<MAX_EVENTS; i++)
    {
        processEventNow(&time, &scheduledEvents[i]);
    }
    processEventNow(&time, &scheduledEvent);    
}