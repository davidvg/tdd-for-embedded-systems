#ifndef _FAKE_TIME_SERVICE_H
#define _FAKE_TIME_SERVICE_H

#include "TimeService.h"

enum {
    TIME_UNKNOWN = -1
};

void FakeTimeService_SetMinute(int minute);
void FakeTimeService_SetDay(int day);

#endif