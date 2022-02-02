extern "C"
{
#include "LightDriverSpy.h"
}

#include "CppUTest/TestHarness.h"

/**
 * - Declare LightDriverSpy_InstallInterface()
 * - Declare LightDriverSpy_Destroy(driver)
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
 * - Implement empty _InstallInterface()
 * - Create src/devices/LightDriver.c and implement _Destroy() to free the driver
 */
TEST(LightDriverSpy, Create)
{
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
}

/**
 * LightDriverSpy: On
 * Here we create the new basic driver structure
 * - Create devices/LightDriverPrivate.h and move the LightDriverStruct from
 *   LightDriver.h file, changing the type member to a string. Remove the
 *   LightDriverType enum from LightDriver.h. Include the private header file
 *   at the end of LightDriver.h
 * - Declare the LightDriver_TurnOn() function
 * - In LightDriverSpy_Create() change type to the string "Spy"
 * - In LightController.h comment out all the switch blocks
 * - In X10LightDriver.c cnange the type X10 to the string "X10"
 * - Implement empty LightDriver_TurnOn()
 * - In LightDriverSpy_Create() change LightDriverSpy to LightDriverSpyStruct inside
 *   the calloc
 * - In the private file create the struct for the interface, containing function
 *   pointers to TurnOn, TurnOff and Destroy
 * - Rename LightDriverSpy_TurnOn() to turnOn()
 * - Create a static LightDriverInterfaceStruct and add turnOn, 0, 0
 * - In LightDriverSpy_InstallInterface() call LightDriver_SetInterface(&interface)
 * - In LightDriver.h create a pointer to LightDriverInterfaceStruct
 * - In LightDriver.c create a static interface. Use LightDriver_SetInterface()
 *   to set it.
 * - In LightDriver_TurnOn() call interface->turnOn
 */
TEST(LightDriverSpy, On)
{
    LightDriver_TurnOn(driver);
    LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(1));
}

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

