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
//  */
// TEST(X10LightDriver, Create)
// {
//     LONGS_EQUAL(X10, driver->type);
//     LONGS_EQUAL(1, driver->id);
// }

// TEST(X10LightDriver, TurnOn)
// {
//     X10LightDriver_TurnOn(driver);
// }

// TEST(X10LightDriver, TurnOff)
// {
//     X10LightDriver_TurnOff(driver);
// }