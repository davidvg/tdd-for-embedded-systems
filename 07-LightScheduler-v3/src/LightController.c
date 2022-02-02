#include "LightController.h"
#include "LightDriverSpy.h"
#include "X10LightDriver.h"
#include <string.h>


static LightDriver drivers[MAX_LIGHTS];

static void destroy(LightDriver driver)
{
    if (!driver)
        return;

    // switch (driver->type)
    // {
    // case X10:
    //     X10LightDriver_Destroy(driver);
    //     break;
    // case TestLightDriver:
    //     LightDriverSpy_Destroy(driver);        
    // default:
    //     break;
    // }
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
    if ((id < 0) || (id >= MAX_LIGHTS))
        return false;

    destroy(drivers[id]);
    drivers[id] = driver;

    return true;
}


void LightController_TurnOn(int id)
{
    LightDriver driver = drivers[id];
    if (driver == NULL)
        return;

    // switch (driver->type)
    // {
    // case X10:
    //     X10LightDriver_TurnOn(driver);
    //     break;
    // case TestLightDriver:
    //     LightDriverSpy_TurnOn(driver);
    // default:
    //     break;
    // }
}

void LightController_TurnOff(int id)
{
    LightDriver driver = drivers[id];
    if (driver == NULL)
        return;

    // switch (driver->type)
    // {
    // case X10:
    //     X10LightDriver_TurnOff(driver);
    //     break;
    // case TestLightDriver:
    //     LightDriverSpy_TurnOff(driver);    
    // default:
    //     break;
    // }
}