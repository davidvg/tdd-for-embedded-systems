#ifndef _LIGHT_CONTROLLER_H
#define _LIGHT_CONTROLLER_H

#include "LightDriver.h"
#include <stdbool.h>

// TODO Change #define to enum?
#define MAX_LIGHTS  32

void LightController_Create(void);
void LightController_Destroy(void);

bool LightController_Add(int id, LightDriver driver);
bool LightController_Remove(int id);

void LightController_TurnOn(int id);
void LightController_TurnOff(int id);

#endif