#include "LightDriverSpy.h"
#include "LightController.h"

typedef struct LightDriverSpyStruct *LightDriverSpy;
typedef struct LightDriverSpyStruct
{
    LightDriverStruct base;
} LightDriverSpyStruct;

static int states[MAX_LIGHTS];
static int lastState;
static int lastId;

static void update(int id, int state)
{
    states[id] = state;
    lastState = state;
    lastId = id;
}

LightDriver LightDriverSpy_Create(int id)
{
    LightDriverSpy self = calloc(1, sizeof(LightDriverSpy));

    self->base.type = TestLightDriver;
    self->base.id = id;

    return (LightDriver) self;
}

void LightDriverSpy_Reset()
{
    for (int i=0; i<MAX_LIGHTS; i++)
    {
        states[i] = LIGHT_STATE_UNKNOWN;
    }
}

void LightDriverSpy_AddSpiesToController(void)
{
    for (int i=0; i<MAX_LIGHTS; i++)
    {
        LightDriver spy = (LightDriver)LightDriverSpy_Create(i);
        LightController_Add(i, spy);
    }
}

void LightDriverSpy_Destroy(LightDriver driver)
{
    LightDriverSpy self = (LightDriverSpy)driver;
    free(self);
}

void LightDriverSpy_TurnOn(LightDriver driver)
{
    LightDriverSpy self = (LightDriverSpy)driver;
    update(self->base.id, LIGHT_ON);
}

void LightDriverSpy_TurnOff(LightDriver driver)
{
    LightDriverSpy self = (LightDriverSpy)driver;
    update(self->base.id, LIGHT_OFF);
}

int LightDriverSpy_GetState(int id)
{
    return states[id];
}

int LightDriverSpy_GetLastId(void)
{
    return lastId;
}

int LightDriverSpy_GetLastState(void)
{
    return lastState;
}