extern "C"
{
#include "LightDriverSpy.h"
}

#include "CppUTest/TestHarness.h"

/**
 * 
 */
TEST_GROUP(LightDriverSpy)
{
    LightDriver lightDriverSpy;

    void setup()
    {
        LightDriverSpy_Reset();
        lightDriverSpy = LightDriverSpy_Create(1);
    }

    void teardown()
    {
        LightDriverSpy_Destroy(lightDriverSpy);
    }
};

/**
 * LightDriverSpy: Create
 */
TEST(LightDriverSpy, Create)
{
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
}

// /**
//  * LightDriverSpy: On
//  */
// TEST(LightDriverSpy, On)
// {
//     LightDriverSpy_TurnOn(lightDriverSpy);
//     LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(1));
// }

// /**
//  * LightDriverSpy: Off
//  */
// TEST(LightDriverSpy, Off)
// {
//     LightDriverSpy_TurnOff(lightDriverSpy);
//     LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetState(1));
// }

// /**
//  */
// TEST(LightDriverSpy, OtherId)
// {
//     LightDriver otherSpy = LightDriverSpy_Create(7);
//     LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(7));

//     LightDriverSpy_TurnOn(otherSpy);
//     LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(7));
//     LONGS_EQUAL(7, LightDriverSpy_GetLastId());
    
//     LightDriverSpy_Destroy(otherSpy);
// }

// /**
//  * LightDriverSpy: RecordsLastIdLastTurnOn
//  */
// TEST(LightDriverSpy, RecordsLastIdLastTurnOn)
// {
//     LightDriverSpy_TurnOff(lightDriverSpy);
//     LONGS_EQUAL(1, LightDriverSpy_GetLastId());
//     LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetLastState());
// }

// TEST(LightDriverSpy, RecordsLastIdLastTurnOff)
// {
//     LightDriverSpy_TurnOn(lightDriverSpy);
//     LONGS_EQUAL(1, LightDriverSpy_GetLastId());
//     LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetLastState());
// }

