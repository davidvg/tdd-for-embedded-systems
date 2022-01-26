#include "LightDriverSpy.h"

static int lastState;
static int lastId;

LightDriver LightDriverSpy_Create(int id)
{
    lastState = LIGHT_STATE_UNKNOWN;
    lastId = 0;
}

void LightDriverSpy_Reset()
{

}

void LightDriverSpy_Destroy(LightDriver lightdriver)
{

}

void LightDriverSpy_TurnOn(LightDriver lightdriver)
{
    lastState = LIGHT_ON;
    lastId = 1;
}

void LightDriverSpy_TurnOff(LightDriver lightdriver)
{
    lastState = LIGHT_OFF;
    lastId = 1;
}

int LightDriverSpy_GetState(int id)
{
    return lastState;
}

int LightDriverSpy_GetLastId(void)
{
    return lastId;
}

int LightDriverSpy_GetLastState(void)
{
    return lastState;
}