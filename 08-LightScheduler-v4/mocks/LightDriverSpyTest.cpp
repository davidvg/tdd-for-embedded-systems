extern "C"
{
#include "LightDriverSpy.h"
}

#include "CppUTest/TestHarness.h"

/**
 * - LightDriverPrivate.h: add a LightDriverInterface vtable to LightDriverStruct
 * - LightDriverSpy.c: in _Create() set vtable=&interface. The order of the
 *   functions must be changed to avoid undeclared functions. Comment out
 *   _InstallInterface()
 * - LightDriver.c: modify the code to call the interface via the vtable included
 *   in the driver after checking that the driver is valid using if(driver).
 *   Remove _SetInterface() and the static interface
 */
TEST_GROUP(LightDriverSpy)
{
    LightDriver driver;

    void setup()
    {
        LightDriverSpy_Reset();
        driver = LightDriverSpy_Create(1);
    }

    void teardown()
    {
        LightDriver_Destroy(driver);
    }
};

/**
 * LightDriverSpy: Create
 */
TEST(LightDriverSpy, Create)
{
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
}

/**
 * LightDriverSpy: On
 */
TEST(LightDriverSpy, On)
{
    LightDriver_TurnOn(driver);
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(1));
}

/**
 * LightDriverSpy: Off
 */
TEST(LightDriverSpy, Off)
{
    LightDriver_TurnOff(driver);
    LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetState(1));
}

/**
 * LightDriverSpy: OtherId
 * This test should pass
 */
TEST(LightDriverSpy, OtherId)
{
    LightDriver otherSpy = LightDriverSpy_Create(7);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(7));

    LightDriver_TurnOn(otherSpy);
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(7));
    LONGS_EQUAL(7, LightDriverSpy_GetLastId());
    
    LightDriver_Destroy(otherSpy);
}

/**
 * LightDriverSpy: RecordsLastIdLastTurnOn
 * This test should pass
 */
TEST(LightDriverSpy, RecordsLastIdLastTurnOn)
{
    LightDriver_TurnOff(driver);
    LONGS_EQUAL(1, LightDriverSpy_GetLastId());
    LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetLastState());
}

/**
 * LightDriverSpy: RecordsLastIdLastTurnOff
 * This test should pass
 */
TEST(LightDriverSpy, RecordsLastIdLastTurnOff)
{
    LightDriver_TurnOn(driver);
    LONGS_EQUAL(1, LightDriverSpy_GetLastId());
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetLastState());
}

