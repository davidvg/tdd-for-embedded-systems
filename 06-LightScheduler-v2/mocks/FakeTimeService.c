#include "FakeTimeService.h"

static Time fakeTime;
static int period;
static WakeUpCallback callback;


void TimeService_Create(void)
{
    fakeTime.minuteOfDay = -1;
    fakeTime.dayOfWeek = -1;
}

void TimeService_Destroy(void)
{

}

void TimeService_GetTime(Time * time) {
    time->minuteOfDay = fakeTime.minuteOfDay;
    time->dayOfWeek = fakeTime.dayOfWeek;
}

void FakeTimeService_SetMinute(int minute)
{
    fakeTime.minuteOfDay = minute;
}

void FakeTimeService_SetDay(int day)
{
    fakeTime.dayOfWeek = day;
}

WakeUpCallback FakeTimeService_GetAlarmCallback(void)
{
    return callback;
}

int FakeTimeService_GetAlarmPeriod(void)
{
    return period;
}

void TimeService_SetPeriodicAlarmInSeconds(int seconds, WakeUpCallback cb)
{
    period = seconds;
    callback = cb;
}

void TimeService_CancelPeriodicAlarmInSeconds(int seconds, WakeUpCallback cb)
{
    if (period == seconds && callback == cb)
    {
        period = 0;
        callback = NULL;
    }
}