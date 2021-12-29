#ifndef _TIME_SERVICE_H
#define _TIME_SERVICE_H

typedef struct Time {
    int minuteOfDay;
    int dayOfWeek;
} Time;

void TimeService_Create(void);
void TimeService_Destroy(void);
void TimeService_GetTime(Time * time);


#endif