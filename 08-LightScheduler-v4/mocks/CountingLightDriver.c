#include "CountingLightDriver.h"

typedef struct CountingLightDriverStruct * CountingLightDriver;

typedef struct CountingLightDriverStruct
{
    LightDriverStruct base;
    int counter;
} CountingLightDriverStruct;

static void count(LightDriver driver)
{
    CountingLightDriver self = (CountingLightDriver) driver;
    self->counter++;
}

static void destroy(LightDriver driver)
{
    CountingLightDriver self = (CountingLightDriver) driver;
    free(self);
}

static LightDriverInterfaceStruct interface = {
    count,
    count,
    destroy
};

LightDriver CountingLightDriver_Create(int id)
{
    CountingLightDriver driver = calloc(1, sizeof(CountingLightDriverStruct));
    driver->base.vtable = &interface;
    driver->base.id = id;
    driver->base.type = "CountingLightDriver";

    return (LightDriver) driver;
}

int CountingLightDriver_GetCount(LightDriver driver)
{
    CountingLightDriver self = (CountingLightDriver) driver;
    return self->counter;
}