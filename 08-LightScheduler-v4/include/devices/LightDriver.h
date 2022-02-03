#ifndef _LIGHT_DRIVER_H
#define _LIGHT_DRIVER_H


typedef struct LightDriverStruct * LightDriver;

typedef struct LightDriverInterfaceStruct * LightDriverInterface;


void LightDriver_SetInterface(LightDriverInterface i);
void LightDriver_Destroy(LightDriver driver);

void LightDriver_TurnOn(LightDriver driver);
void LightDriver_TurnOff(LightDriver driver);

int LightDriver_GetId(LightDriver driver);
const char * LightDriver_GetType(LightDriver driver);

#include "LightDriverPrivate.h"

#endif