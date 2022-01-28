#ifndef _LIGHT_DRIVER_SPY_H
#define _LIGHT_DRIVER_SPY_H

#include "LightDriver.h"
#include "LightController.h"

enum{
    LIGHT_STATE_UNKNOWN=-1,
    LIGHT_ID_UNKNOWN=-1,
    LIGHT_ON=0,
    LIGHT_OFF=1,
};

LightDriver LightDriverSpy_Create(int id);
void LightDriverSpy_Reset(void);
void LightDriverSpy_AddSpiesToController(void);
void LightDriverSpy_Destroy(LightDriver driver);

void LightDriverSpy_TurnOn(LightDriver driver);
void LightDriverSpy_TurnOff(LightDriver driver);

int LightDriverSpy_GetState(int id);
int LightDriverSpy_GetLastId(void);
int LightDriverSpy_GetLastState(void);

#endif