#include "LightScheduler.h"
#include "TimeService.h"
#include "LightController.h"

enum {
    UNUSED = -1,
};

typedef struct {
    int id;
    int minuteOfDay;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvent;

void LightScheduler_Create(void)
{
    scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{}

void LightScheduler_WakeUp(void)
{
    Time time;
    TimeService_GetTime(&time);
    
    // No valid ID passed
    if (scheduledEvent.id == UNUSED)
        return;
    // Not time yet
    if (scheduledEvent.minuteOfDay != time.minuteOfDay)
        return;

    LightController_On(scheduledEvent.id);
}

void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
    scheduledEvent.id = id;
    scheduledEvent.minuteOfDay = minuteOfDay;
}