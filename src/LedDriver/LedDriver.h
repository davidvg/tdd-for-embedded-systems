#ifndef _LED_DRIVER_H
#define _LED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
// #include <inttypes.h>
#include "RuntimeError.h"

void LedDriver_Create(uint16_t *address);
void LedDriver_Destroy(void);
void LedDriver_TurnOn(int ledNumber);
void LedDriver_TurnOff(int ledNumber);
void LedDriver_TurnAllOn(void);
void LedDriver_TurnAllOff(void);
bool LedDriver_IsOn(int ledNumber);
bool LedDriver_IsOff(int ledNumber);

#endif