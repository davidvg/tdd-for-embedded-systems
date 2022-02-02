#ifndef _LIGHT_DRIVER_H
#define _LIGHT_DRIVER_H

typedef enum LightDriverType
{
    TestLightDriver,
    X10,
} LightDriverType;

typedef struct LightDriverStruct * LightDriver;

typedef struct LightDriverStruct
{
    LightDriverType type;
    int id;
} LightDriverStruct;


void LightDriver_Destroy(LightDriver driver);


#endif