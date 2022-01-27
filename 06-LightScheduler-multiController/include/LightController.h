#ifndef _LIGHT_CONTROLLER_H
#define _LIGHT_CONTROLLER_H

#include "LightDriver.h"

// TODO Change #define to enum?
#define MAX_LIGHTS  32

void LightController_Create(void);
void LightController_Destroy(void);

void LightController_Add(int id, LightDriver driver);

void LightController_TurnOn(int id);
void LightController_TurnOff(int id);

#endif