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

static void processEventNow(Time *time, ScheduledLightEvent *lightEvent)
{
    int reactionDay = lightEvent->day;
    int today = time->dayOfWeek;

    if (lightEvent->id == UNUSED)
        return;
    if (reactionDay != EVERYDAY && reactionDay != today)
        return;
    if (lightEvent->minuteOfDay != time->minuteOfDay)
        return;
    
    operateLight(lightEvent);
}

void LightScheduler_Create(void)
{
    scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{}

void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
    scheduledEvent.id = id;
    scheduledEvent.day = day;
    scheduledEvent.minuteOfDay = minuteOfDay;
    scheduledEvent.event = TURN_ON;
}

void LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
    scheduledEvent.id = id;
    scheduledEvent.day = day;
    scheduledEvent.minuteOfDay = minuteOfDay;
    scheduledEvent.event = TURN_OFF;
}

void LightScheduler_WakeUp(void)
{
    Time time;
    TimeService_GetTime(&time);
    
    processEventNow(&time, &scheduledEvent);
}