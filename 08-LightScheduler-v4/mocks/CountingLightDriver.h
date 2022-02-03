#ifndef _COUNTING_LIGHT_DRIVER_H_
#define _COUNTING_LIGHT_DRIVER_H_

#include "LightDriver.h"

LightDriver CountingLightDriver_Create(int id);

int CountingLightDriver_GetCount(LightDriver driver);

#endif