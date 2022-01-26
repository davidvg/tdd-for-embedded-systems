#ifndef _LIGHT_DRIVER_SPY_H
#define _LIGHT_DRIVER_SPY_H

#include "LightDriver.h"

enum{
    LIGHT_STATE_UNKNOWN=-1,
    LIGHT_ID_UNKNOWN=-1,
    LIGHT_ON=0,
    LIGHT_OFF=1,
};

LightDriver LightDriverSpy_Create(int id);
void LightDriverSpy_Reset();
void LightDriverSpy_Destroy(LightDriver lightdriver);

void LightDriverSpy_TurnOn(LightDriver lightdriver);
void LightDriverSpy_TurnOff(LightDriver lightdriver);

int LightDriverSpy_GetState(int id);
int LightDriverSpy_GetLastId(void);
int LightDriverSpy_GetLastState(void);

#endif