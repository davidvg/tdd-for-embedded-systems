extern "C"
{
    #include "LightController.h"
    #include "LightDriverSpy.h"
    #include <stdbool.h>
}

#include "CppUTest/TestHarness.h"

/**
 * - Create LightDriverSpy_AddSpiesToController(); inside it create MAX_LIGHTS
 *   spies and call LightController_Add(i, spy)
 */
TEST_GROUP(LightController)
{
    LightDriver spy;

    void setup()
    {
      LightController_Create();
      LightDriverSpy_AddSpiesToController();
    }

    void teardown()
    {
       LightController_Destroy();
    }
};

/**
 * - Create src/LightController.c
 * - Rename LightController_On() to LightController_TurnOn(id)
 * - Update LightController_On() and _Off() inside:
 *      - LightSchedulerTest.cpp
 *      - LightControllerSpyTest.cpp
 * - In LightController.c create static LightDriver drivers[MAX_LIGHTS]; in
 *   Create() initialize it using memset
 * - In _Add() store the driver in drivers[id]
 * - In _Destroy() call LightDriverSpy_Destroy(driver) for each driver
 * - In _TurnOn() call LightDriverSpy_TurnOn()
 */
TEST(LightController, TurnOn)
{
	LightController_TurnOn(7);
	LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(7));
}

TEST(LightController, TurnOff)
{
	LightController_TurnOff(1);
	LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetState(1));
}

/**
 * LightController: AddingDriverDestroysPreviousAndDoesNotLeak
 * - Call LightDriverSpy_Destroy(drivers[id]) before adding the driver to the array
 */
TEST(LightController, AddingDriverDestroysPreviousAndDoesNotLeak)
{
    spy = LightDriverSpy_Create(1);
    LightController_Add(1, spy);
    LightController_Destroy();
}

/**
 * LightController: AllDriversDestroyed
 * Drivers are added in setup(); here all the drivers are overwritten. If they
 * are not destroyed, memory leaks happen. To detect a success when adding a
 * driver, a bool is returned by LightController_Add()
 * - Modify LightController_Add() to return bool, and return true
 */
TEST(LightController, AllDriversDestroyed)
{
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		spy = LightDriverSpy_Create(i);
		LONGS_EQUAL(true, LightController_Add(i, spy));
	}
}

// TEST(LightController, ValidIdLowerRange)
// {
// 	LightDriver spy = LightDriverSpy_Create(0);
// 	LONGS_EQUAL(TRUE, LightController_Add(0, spy));
// }

// TEST(LightController, ValidIdUpperRange)
// {
// 	LightDriver spy = LightDriverSpy_Create(MAX_LIGHTS);
// 	LONGS_EQUAL(TRUE, LightController_Add(MAX_LIGHTS-1, spy));
// }

// TEST(LightController, InValidIdBeyondUpperRange)
// {
// 	LightDriver spy = LightDriverSpy_Create(MAX_LIGHTS);
// 	LONGS_EQUAL(FALSE, LightController_Add(MAX_LIGHTS, spy));
// 	free(spy);
// }

// TEST(LightController, NonAddedLightDoesNothing)
// {
//     LightController_Add(1, NULL);
//     LightController_TurnOn(1);
//     LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
//     LightController_TurnOff(1);
//     LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
// }



