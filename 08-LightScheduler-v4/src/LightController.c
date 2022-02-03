#include "LightController.h"
#include "LightDriverSpy.h"
#include "X10LightDriver.h"
#include <string.h>


static LightDriver drivers[MAX_LIGHTS];

static bool isIdOutOfBounds(int id)
{
    return (id < 0) || (id >= MAX_LIGHTS);
}

static bool isInvalidDriver(LightDriver driver)
{
    return driver == NULL;
}

void LightController_Create(void)
{
    memset(drivers, 0, sizeof(drivers));
}

void LightController_Destroy(void)
{
    for (int i=0; i<MAX_LIGHTS; i++)
    {
        LightDriver driver = drivers[i];
        LightDriver_Destroy(driver);
        drivers[i] = NULL;
    }
}

bool LightController_Add(int id, LightDriver driver)
{
    if (isIdOutOfBounds(id))
        return false;

    if (isInvalidDriver(driver))
        return false;

    LightDriver_Destroy(drivers[id]);
    drivers[id] = driver;

    return true;
}

bool LightController_Remove(int id)
{
    if (isIdOutOfBounds(id))
        return false;

    if (isInvalidDriver(drivers[id]))
        return false;

    LightDriver_Destroy(drivers[id]);
    drivers[id] = NULL;
    return true;
}

void LightController_TurnOn(int id)
{
    LightDriver driver = drivers[id];
    if (isInvalidDriver(driver))
        return;

    LightDriver_TurnOn(driver);
}

void LightController_TurnOff(int id)
{
    LightDriver driver = drivers[id];
    if (isInvalidDriver(driver))
        return;

    LightDriver_TurnOff(driver);
}