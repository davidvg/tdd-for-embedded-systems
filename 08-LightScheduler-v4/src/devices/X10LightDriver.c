#include "X10LightDriver.h"

typedef struct X10LightDriverStruct *X10LightDriver;
typedef struct X10LightDriverStruct
{
    LightDriverStruct base;
    X10_HouseCode house;
    int unit;
} X10LightDriverStruct;

LightDriver X10LightDriver_Create(int id, X10_HouseCode house, int unit)
{
    X10LightDriver self = calloc(1, sizeof(X10LightDriverStruct));
    self->base.type = "X10";
    self->base.id = id;
    self->house = house;
    self->unit = unit;

    return (LightDriver)self;
}

void X10LightDriver_Destroy(LightDriver driver)
{
    // X10LightDriver self = (X10LightDriver)driver;
    free(driver);
}

void X10LightDriver_TurnOn(LightDriver driver)
{
    
}

void X10LightDriver_TurnOff(LightDriver driver)
{
    
}