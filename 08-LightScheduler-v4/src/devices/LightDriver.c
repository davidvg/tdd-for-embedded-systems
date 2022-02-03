#include "LightDriver.h"


void LightDriver_Destroy(LightDriver driver)
{
    if (driver)
        driver->vtable->Destroy(driver);
}

void LightDriver_TurnOn(LightDriver driver)
{
    if (driver)
        driver->vtable->TurnOn(driver);
}

void LightDriver_TurnOff(LightDriver driver)
{
    if (driver)
        driver->vtable->TurnOff(driver);
}

int LightDriver_GetId(LightDriver driver)
{
    return driver->id;
}

const char * LightDriver_GetType(LightDriver driver)
{
    return driver->type;
}