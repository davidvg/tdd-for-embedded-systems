#ifndef _LIGHT_DRIVER_PRIVATE_H
#define _LIGHT_DRIVER_PRIVATE_H


typedef struct LightDriverStruct
{
    const char * type;
    int id;
} LightDriverStruct;

typedef struct LightDriverInterfaceStruct
{
    void (*TurnOn)(LightDriver);
    void (*TurnOff)(LightDriver);
    void (*Destroy)(LightDriver);
} LightDriverInterfaceStruct;

#endif