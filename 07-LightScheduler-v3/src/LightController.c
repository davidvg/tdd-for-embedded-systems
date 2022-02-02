#include "LightController.h"
#include "LightDriverSpy.h"
#include "X10LightDriver.h"
#include <string.h>


static LightDriver drivers[MAX_LIGHTS];

static bool isIdOutOfBounds(int id)
{
    return (id < 0) || (id >= MAX_LIGHTS);
}

// TODO Check if this method is needed
static void destroy(LightDriver driver)
{
    if (!driver)
        return;

    LightDriver_Destroy(driver);
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
        destroy(driver);   
        drivers[i] = NULL;
    }
}

bool LightController_Add(int id, LightDriver driver)
{
    if (isIdOutOfBounds(id))
        return false;

    destroy(drivers[id]);
    drivers[id] = driver;

    return true;
}

bool LightController_Remove(int id)
{
    if (isIdOutOfBounds(id))
        return false;

    if (drivers[id] == NULL)
        return false;

    destroy(drivers[id]);
    drivers[id] = NULL;
    return true;
}

void LightController_TurnOn(int id)
{
    LightDriver driver = drivers[id];
    if (driver == NULL)
        return;

    LightDriver_TurnOn(driver);
}

void LightController_TurnOff(int id)
{
    LightDriver driver = drivers[id];
    if (driver == NULL)
        return;

    LightDriver_TurnOff(driver);
}