extern "C"
{
    #include "X10LightDriver.h"
}

#include "CppUTest/TestHarness.h"

// /**
//  * Test Group
//  */
// TEST_GROUP(X10LightDriver)
// {
//     LightDriver driver;

//     void setup()
//     {
//       driver = X10LightDriver_Create(1, X10_A, 12);
//     }

//     void teardown()
//     {
//        X10LightDriver_Destroy(driver);
//     }
// };

// /**
//  * X10LightDriver: Create
//  * - Create LightDriver_GetType() and return driver->type
//  * - Create LightDriver_GetId() and return driver->id
//  */
// TEST(X10LightDriver, Create)
// {
//     LONGS_EQUAL(1, LightDriver_GetId(driver));
//     STRCMP_EQUAL("X10", LightDriver_GetType(driver));
// }

// TEST(X10LightDriver, TurnOn)
// {
//     X10LightDriver_TurnOn(driver);
// }

// TEST(X10LightDriver, TurnOff)
// {
//     X10LightDriver_TurnOff(driver);
// }