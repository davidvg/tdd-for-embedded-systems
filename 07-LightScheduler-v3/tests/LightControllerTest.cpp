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

// TEST(LightController, TurnOff)
// {
// 	LightController_TurnOff(1);
// 	LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetState(1));
// }

// /**
//  * LightController: AddingDriverDestroysPreviousAndDoesNotLeak
//  */
// TEST(LightController, AddingDriverDestroysPreviousAndDoesNotLeak)
// {
//     spy = LightDriverSpy_Create(1);
//     LightController_Add(1, spy);
//     LightController_Destroy();
// }

// /**
//  * LightController: AllDriversDestroyed
//  */
// TEST(LightController, AllDriversDestroyed)
// {
// 	for (int i = 0; i < MAX_LIGHTS; i++)
// 	{
// 		spy = LightDriverSpy_Create(i);
// 		LONGS_EQUAL(true, LightController_Add(i, spy));
// 	}
// }

// /**
//  * LightController: ValidIdLowerRange
//  */
// TEST(LightController, ValidIdLowerRange)
// {
// 	spy = LightDriverSpy_Create(0);
// 	LONGS_EQUAL(true, LightController_Add(0, spy));
// }

// /**
//  * LightController: ValidIdUpperRange
//  */
// TEST(LightController, ValidIdUpperRange)
// {
// 	spy = LightDriverSpy_Create(MAX_LIGHTS);
// 	LONGS_EQUAL(true, LightController_Add(MAX_LIGHTS-1, spy));
// }

// /**
//  * LightController: InValidIdBeyondUpperRange
//  */
// TEST(LightController, InValidIdBeyondUpperRange)
// {
// 	spy = LightDriverSpy_Create(MAX_LIGHTS);
// 	LONGS_EQUAL(false, LightController_Add(MAX_LIGHTS, spy));
// 	free(spy);
// }

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



