#include "LightDriver.h"

static LightDriverInterface interface;


void LightDriver_SetInterface(LightDriverInterface i)
{
    interface = i;
}

void LightDriver_Destroy(LightDriver driver)
{
    free(driver); // TODO Change to interface method
}

void LightDriver_TurnOn(LightDriver driver)
{
    interface->TurnOn(driver);
}

void LightDriver_TurnOff(LightDriver driver)
{
    interface->TurnOff(driver);
}