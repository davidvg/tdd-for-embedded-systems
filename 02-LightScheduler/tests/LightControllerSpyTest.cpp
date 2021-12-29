extern "C"
{
    #include "LightControllerSpy.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(LightControllerSpy)
{
    void setup()
    {
        LightController_Create();
    }

    void teardown()
    {
        LightController_Destroy();
    }
};

/**
 * LightControllerSpy: Create
 * This test should pass if (LightScheduler, NoChangeToLightsDuringInitialization)
 * passed.
 */
TEST(LightControllerSpy, Create)
{
    LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

/**
 * LightControllerSpy: RememberTheLastLightIdControlled
 * - In LightController.h, declare LightController_On(int) and
 *   LightController_On(int). These are the INTERFACE.
 * - In LightControllerSpy.c, implement these functions to substitute the
 *   production code (the LightController.c file, which doesn't exist). They
 *   update lastId and lastState with the appropiate values.
 * - Add LIGHT_ON and LIGHT_OFF to the enum in LightControllerSpy.h
 */
TEST(LightControllerSpy, RememberTheLastLightIdControlled)
{
    LightController_On(10);
    LONGS_EQUAL(10, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_ON, LightControllerSpy_GetLastState());

    LightController_On(12);
    LightController_Off(12);
    LONGS_EQUAL(12, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_OFF, LightControllerSpy_GetLastState());
}