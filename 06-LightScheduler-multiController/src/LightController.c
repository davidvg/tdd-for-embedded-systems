#include "LightController.h"
#include "LightDriverSpy.h"
#include <string.h>


static LightDriver drivers[MAX_LIGHTS];

void LightController_Create(void)
{
    memset(drivers, 0, sizeof(drivers));
}

void LightController_Destroy(void)
{
    for (int i=0; i<MAX_LIGHTS; i++)
    {
        LightDriverSpy_Destroy(drivers[i]);
        drivers[i] = NULL;
    }
}

bool LightController_Add(int id, LightDriver driver)
{
    if ((id < 0) || (id >= MAX_LIGHTS))
        return false;

    LightDriverSpy_Destroy(drivers[id]);

    drivers[id] = driver;

    return true;
}


void LightController_TurnOn(int id)
{
    LightDriver driver = drivers[id];
    if (driver == NULL)
        return;

    LightDriverSpy_TurnOn(driver);
}

void LightController_TurnOff(int id)
{
    LightDriver driver = drivers[id];
    if (driver == NULL)
        return;

    LightDriverSpy_TurnOff(driver);
}