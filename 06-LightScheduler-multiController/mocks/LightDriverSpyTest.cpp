extern "C"
{
#include "LightDriverSpy.h"
}

#include "CppUTest/TestHarness.h"

/**
 * - Create mocks/LightDriverSpy.h and .c
 * - Create include/LightDriver.h
 * - Declare LightDriverSpy_Create() and _Destroy()
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
 * - In LightDriverSpy.h create an enum with light states.
 * - Declare and implement _GetState() to return LIGHT_STATE_UNKNOWN
 */
TEST(LightDriverSpy, Create)
{
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
}

/**
 * LightDriverSpy: On
 * - Declare LightDriverSpy_TurnOn(ld)
 * - Add LIGHT_ON to the enum
 * - in Spy.c create a static int lastState; initalize it in Create() to
 *   LIGHT_STATE_UNKNOWN and make _GetStaet() return it.
 * - In _TurnOn() change lastState to LIGHT_ON
 */
TEST(LightDriverSpy, On)
{
    LightDriverSpy_TurnOn(lightDriverSpy);
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(1));
}

/**
 * LightDriverSpy: Off
 * - Like the TurnOn case
 */
TEST(LightDriverSpy, Off)
{
    LightDriverSpy_TurnOff(lightDriverSpy);
    LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetState(1));
}

/**
 * - Include LightController.h
 * - Create static int states[MAX_LIGHTS] and initialize to UNKNOWN in Reset
 * - Add LightDriverSpy_Reset() to setup()
 * - Modify _GetState(id) to return states[id]
 * 
 * ### LightDriverSpy Implementation ###
 * - In LightDriverSpy.c create struct LightDriverSpyStruct to include a
 *   LightDriverStruct base.
 * - Create *LightDriverSpy, a pointer to struct LightDriverSpyStruct
 * - In Create() allocate memory for the struct. Return it as a LightDriver.
 *   Initialize the allocated object's base: id=id and type=TestLightDriver
 * - In Destroy() free memory
 * - In TurnOn() cast driver to LightDriverSpy and update states[self->base.id].
 *   Update lastId with base.id
 * - Do the same in TurnOff()
 */
TEST(LightDriverSpy, OtherId)
{
    LightDriver otherSpy = LightDriverSpy_Create(7);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(7));

    LightDriverSpy_TurnOn(otherSpy);
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(7));
    LONGS_EQUAL(7, LightDriverSpy_GetLastId());
    
    LightDriverSpy_Destroy(otherSpy);
}

/**
 * LightDriverSpy: RecordsLastIdLastTurnOn
 * - Declare _GetLastId() and _GetLastState()
 */
TEST(LightDriverSpy, RecordsLastIdLastTurnOn)
{
    LightDriverSpy_TurnOff(lightDriverSpy);
    LONGS_EQUAL(1, LightDriverSpy_GetLastId());
    LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetLastState());
}

TEST(LightDriverSpy, RecordsLastIdLastTurnOff)
{
    LightDriverSpy_TurnOn(lightDriverSpy);
    LONGS_EQUAL(1, LightDriverSpy_GetLastId());
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetLastState());
}

