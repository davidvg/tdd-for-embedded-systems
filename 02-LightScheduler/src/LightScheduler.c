#include "LightScheduler.h"
#include "TimeService.h"
#include "LightController.h"

enum {
    UNUSED = -1,
    TURN_ON = 0,
    TURN_OFF,
};

typedef struct {
    int id;
    Day day;
    int minuteOfDay;
    int event;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvent;

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
    scheduledEvent.id = id;
    scheduledEvent.day = day;
    scheduledEvent.minuteOfDay = minute;
    scheduledEvent.event = event;
}

void LightScheduler_Create(void)
{
    scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{}

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
    
    processEventNow(&time, &scheduledEvent);
}