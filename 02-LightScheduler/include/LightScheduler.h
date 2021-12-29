#ifndef _LIGHT_SCHEDULER_H
#define _LIGHT_SCHEDULER_H

typedef enum  {
    NONE=-1,
    SUNDAY=1,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
} Day;

void LightScheduler_Create(void);
void LightScheduler_Destroy(void);

#endif