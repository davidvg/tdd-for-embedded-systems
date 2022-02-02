extern "C"
{
    #include "LightController.h"
    #include "LightDriverSpy.h"
    #include "X10LightDriver.h"
    #include <stdbool.h>
}

#include "CppUTest/TestHarness.h"

/**
 */
TEST_GROUP(LightController)
{
    LightDriver spy;

    void setup()
    {
      LightController_Create();
      LightDriverSpy_AddSpiesToController();
      LightDriverSpy_InstallInterface();
      LightDriverSpy_Reset();
    }

    void teardown()
    {
       LightController_Destroy();
    }
};

/**
 * LightController: CreateDestroy
 * - LightController.c: in destroy() substitute the switch block with
 *   LightDriver_Destroy(driver)
 */
TEST(LightController, CreateDestroy)
{

}

/**
 * LightController: AddingDriverDestroysPreviousAndDoesNotLeak
 */
TEST(LightController, AddingDriverDestroysPreviousAndDoesNotLeak)
{
    spy = LightDriverSpy_Create(1);
    LightController_Add(1, spy);
    // LightController_Destroy();
}

/**
 * LightController: TurnOn
 * - In LightController_TurnOn() call LightDriver_TurnOn(driver)
 */
TEST(LightController, TurnOn)
{
	LightController_TurnOn(7);
	LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(7));
}

/**
 * LightController: TurnOff
 * - In LightController_TurnOff() call LightDriver_TurnOff(driver)
 */
TEST(LightController, TurnOff)
{
	LightController_TurnOff(1);
	LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetState(1));
}

/**
 * LightController: AllDriversDestroyed
 * This test should pass
 */
TEST(LightController, AllDriversDestroyed)
{
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		spy = LightDriverSpy_Create(i);
		LONGS_EQUAL(true, LightController_Add(i, (LightDriver)spy));
	}
}

/**
 * LightController: ValidIdLowerRange
 * This test should pass
 */
TEST(LightController, ValidIdLowerRange)
{
	spy = LightDriverSpy_Create(0);
	LONGS_EQUAL(true, LightController_Add(0, (LightDriver)spy));
}

/**
 * LightController: ValidIdUpperRange
 * This test should pass
 */
TEST(LightController, ValidIdUpperRange)
{
	spy = LightDriverSpy_Create(MAX_LIGHTS);
	LONGS_EQUAL(true, LightController_Add(MAX_LIGHTS-1, (LightDriver)spy));
}

/**
 * LightController: InValidIdBeyondUpperRange
 * This test should pass
 */
TEST(LightController, InValidIdBeyondUpperRange)
{
	spy = LightDriverSpy_Create(MAX_LIGHTS);
	LONGS_EQUAL(false, LightController_Add(MAX_LIGHTS, (LightDriver)spy));
	free(spy);
}

/**
 * LightController: RemoveExistingLightDriverSucceeds
 * - Create LightController_Remove(id)
 * - Check if id is out of bounds and return false if so
 * - destroy drivers[id] and make drivers[id]=NULL, and return true
 */
TEST(LightController, RemoveExistingLightDriverSucceeds)
{
    CHECK(LightController_Remove(10));
}

/**
 * LightController: RemovedLightDoesNothing
 * This test should pass
 */
TEST(LightController, RemovedLightDoesNothing)
{
    LightController_Remove(1);
    LightController_TurnOn(1);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
    LightController_TurnOff(1);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
}

/**
 * LightController: RemoveNonExistingLightDriverFails
 * - In LightController_Remove() check if driver==NULL and return false
 */
TEST(LightController, RemoveNonExistingLightDriverFails)
{
    CHECK(LightController_Remove(10));
    CHECK(LightController_Remove(10) == false);
}

/**
 * LightController: RejectsNullDrivers
 * - In _Add() check if driver==NULL and return false
 */
TEST(LightController, RejectsNullDrivers)
{
    LONGS_EQUAL(false, LightController_Add(1, NULL));
}

/**
 * LightController: TurnOnNonExistingDriverDoesNothing
 * This test should pass
 */
TEST(LightController, TurnOnNonExistingDriverDoesNothing)
{
    LightController_Remove(7);
    LightController_TurnOn(7);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(7));
}

/**
 * LightController: TurnOffNonExistingDriverDoesNothing
 * This test should pass
 */
TEST(LightController, TurnOffNonExistingDriverDoesNothing)
{
    LightController_Remove(7);
    LightController_TurnOff(7);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(7));
}

// /**
//  * LightController: NonAddedLightDoesNothing
//  */
// TEST(LightController, NonAddedLightDoesNothing)
// {
//     LightController_Add(1, NULL);
//     LightController_TurnOn(1);
//     LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
//     LightController_TurnOff(1);
//     LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
// }



