extern "C"
{
    #include "X10LightDriver.h"
}

#include "CppUTest/TestHarness.h"

/**
 * Test Group
 * - Create include/devices/X10LightDriver.h and include LightDriver.h
 * - Create X10LightDriver_Create() and _Destroy()
 * - In the header file create an enum with house codes
 * - Update paths in the Makefile
 */
TEST_GROUP(X10LightDriver)
{
    LightDriver driver;

    void setup()
    {
      driver = X10LightDriver_Create(1, X10_A, 12);
    }

    void teardown()
    {
       X10LightDriver_Destroy(driver);
    }
};

/**
 * X10LightDriver: Create
 * - Create src/devices/X10LightDriver.c; inside create the structs like in the
 *   spy: X10LightDriverStruct and *X10LightDriver
 * - In Create() use calloc to allocate memory for a X10LightDriverStruct and
 *   return the X10LightDriver as a LightDriver
 * - Add X10 to the driver types enum in LightDriver.h
 * - In _Create() make self->base.type = X10
 * - In _Create() make self->base.id = id
//  * - In _Destroy() cast the driver to X10LightDriver and free it
 */
TEST(X10LightDriver, Create)
{
    LONGS_EQUAL(X10, driver->type);
    LONGS_EQUAL(1, driver->id);
}

TEST(X10LightDriver, TurnOn)
{
    X10LightDriver_TurnOn(driver);
}

TEST(X10LightDriver, TurnOff)
{
    X10LightDriver_TurnOff(driver);
}