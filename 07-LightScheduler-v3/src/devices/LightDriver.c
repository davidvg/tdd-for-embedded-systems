#include "LightDriver.h"

static LightDriverInterface interface;


void LightDriver_SetInterface(LightDriverInterface i)
{
    interface = i;
}

void LightDriver_Destroy(LightDriver driver)
{
    interface->Destroy(driver);
}

void LightDriver_TurnOn(LightDriver driver)
{
    interface->TurnOn(driver);
}

void LightDriver_TurnOff(LightDriver driver)
{
    interface->TurnOff(driver);
}

int LightDriver_GetId(LightDriver driver)
{
    return driver->id;
}

const char * LightDriver_GetType(LightDriver driver)
{
    return driver->type;
}