extern "C"
{
    #include "LightScheduler.h"
    #include "LightControllerSpy.h"
    // #include "FakeTimeService.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(LightScheduler)
{
    void setup()
    {
        LightScheduler_Create();
        LightController_Create();
    }

    void teardown()
    {
        LightScheduler_Destroy();
        LightController_Create();
    }
};

/**
 * LightScheduler: NoChangeToLightsDuringInitialization
 * - Create src/LightScheduler.h and declare the Create() and Destroy() functions.
 * - Create src/LightScheduler.c and implement Create() and Destroy() with empty
 *   bodies.
 * - Create include/LightController.h interface and declare the Create() and
 *   Destroy() functions.
 * - Create test/LightControllerSpy.h and declare LightControllerSpy_GetLastId()
 *   and LightControllerSpy_GetLastState(). Create the ENUM with the required
 *   elements to pass the test.
 * - Create LightControllerSpy.c and create two static int, lastId and lastState.
 *   In LightControllerSpy_Create(), initialize lastId and lastState.
 * - In LightControllerSpy.c implement LightControllerSpy_GetLastId() and
 *   LightControllerSpy_GetLastState() to return lastId and lastState.
 */
TEST(LightScheduler, NoChangeToLightsDuringInitialization)
{
    LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}