#ifndef _LIGHT_SCHEDULER_H
#define _LIGHT_SCHEDULER_H

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

#define FALSE   0
#define TRUE    1

void LightScheduler_Create(void);
void LightScheduler_Destroy(void);
void LightScheduler_WakeUp(void);
void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay);
void LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay);

#endif