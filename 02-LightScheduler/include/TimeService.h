#ifndef _TIME_SERVICE_H
#define _TIME_SERVICE_H

typedef struct Time {
    int minuteOfDay;
    int dayOfWeek;
} Time;

typedef void (*WakeUpCallback) (void);

void TimeService_Create(void);
void TimeService_Destroy(void);
void TimeService_GetTime(Time * time);
void TimeService_SetPeriodicAlarmInSeconds(int seconds, WakeUpCallback cb);
void TimeService_CancelPeriodicAlarmInSeconds(int seconds, WakeUpCallback cb);

#endif