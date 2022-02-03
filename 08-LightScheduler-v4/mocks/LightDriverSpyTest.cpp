extern "C"
{
#include "LightDriverSpy.h"
}

#include "CppUTest/TestHarness.h"

/**
 */
TEST_GROUP(LightDriverSpy)
{
    LightDriver driver;

    void setup()
    {
        LightDriverSpy_Reset();
        driver = LightDriverSpy_Create(1);
        LightDriverSpy_InstallInterface();
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

