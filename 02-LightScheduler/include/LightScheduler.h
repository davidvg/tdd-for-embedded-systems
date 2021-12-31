#ifndef _LIGHT_SCHEDULER_H
#define _LIGHT_SCHEDULER_H

#include "bool.h"

typedef enum  {
    NONE = -1,
    SUNDAY = 1,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    EVERYDAY = 10,
    WEEKEND,
    WEEKDAY,
} Day;

enum {
    LS_OK,
    LS_TOO_MANY_EVENTS,
    LS_ID_OUT_OF_BOUNDS,
};

void LightScheduler_Create(void);
void LightScheduler_Destroy(void);
void LightScheduler_WakeUp(void);
int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay);
int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay);
void LightScheduler_ScheduleRemove(int i, Day day, int minuteOfDay);

#endif