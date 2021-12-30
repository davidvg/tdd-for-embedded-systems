#include "LightControllerSpy.h"

static int lastId;
static int lastState;
static int lights[MAX_LIGHTS];

/*** Functions specific to the Spy ***/

int LightControllerSpy_GetLastId(void)
{
    return lastId;
}

int LightControllerSpy_GetLastState(void)
{
    return lastState;
}

int LightControllerSpy_GetLightState(int id)
{
    return lights[id];
}

/*** Override the Interface ***/

void LightController_Create(void)
{
    lastId = LIGHT_ID_UNKNOWN;
    lastState = LIGHT_STATE_UNKNOWN;

    for (int i=0; i<MAX_LIGHTS; i++)
    {
        lights[i] = LIGHT_STATE_UNKNOWN;
    }
}

void LightController_Destroy(void)
{

}

void LightController_On(int id)
{
    lastId = id;
    lastState = LIGHT_ON;
    lights[id] = LIGHT_ON;
}

void LightController_Off(int id)
{
    lastId = id;
    lastState = LIGHT_OFF;

    lights[id] = LIGHT_OFF;

}