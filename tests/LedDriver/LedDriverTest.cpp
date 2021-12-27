#include "CppUTest/TestHarness.h"

extern "C"
{
    #include "LedDriver.h"
    // #include <stdint.h>
    #include "RuntimeErrorStub.h"
}


static uint16_t virtualLeds = 0xffff;

TEST_GROUP(LedDriver)
{
    void setup()
    {
        LedDriver_Create(&virtualLeds);
    }

    void teardown()
    {
    }
};


TEST(LedDriver, LedsOffAfterCreate)
{
    virtualLeds = 0xffff;
    LedDriver_Create(&virtualLeds);
    LONGS_EQUAL(0x0, virtualLeds);
}

TEST(LedDriver, TurnOnLedOne) 
{
    LedDriver_TurnOn(1);
    LONGS_EQUAL(0x1, virtualLeds);
}

TEST(LedDriver, TurnOffLedOne) 
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);
    LONGS_EQUAL(0x0, virtualLeds);
}

TEST(LedDriver, TurnOnMultipleLeds)
{
    LedDriver_TurnOn(8);
    LedDriver_TurnOn(9);
    LONGS_EQUAL(0x0180, virtualLeds);
}

TEST(LedDriver, TurnAllOn)
{
    LedDriver_TurnAllOn();
    LONGS_EQUAL(0xffff, virtualLeds);
}

TEST(LedDriver, TurnAllOff)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnAllOff();
    LONGS_EQUAL(0x0000, virtualLeds);
}

TEST(LedDriver, TurnOffAnyLed)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(8);
    LONGS_EQUAL(0xff7f, virtualLeds); // 1111.0111
}
/**
 * The datasheet for the Led Driver says that the status register for the
 * driver is not readable. This means that if we want to obtain the current
 * status for the LED bank, we can't get it from the driver.
 * 
 * The following test checks that the current state is obtained. 
 * 
 * After LedDriver_Create(&virtualLeds), virtualLeds is 0x0000.
 * virtualLeds is passed by address, so it gets updated when modifications
 * are done to the LED register in LedDriver.c
 * If it's overwritten, this update does not happen anymore.
 */
TEST(LedDriver, LedRegisterIsNotReadable)
{
    // After calling LedDriver_Create(&virtualLeds), virtualLeds=0x0000.
    // Overwrite virtualLeds with a value whose 8-th bit is 0.
    virtualLeds = 0x0101;
    // When LedDriver_TurnOn(8) is called, a 1 is written to the 8-th bit,
    // and the remaining LEDs are off, so virtualLeds should be 0x0080. If 
    // the state was readable, this would be the value of virtualLeds after
    // turning the LED on.
    LedDriver_TurnOn(8);
    // As the state is not readable, the 8-th bit has been set and the current
    // value for virtualLeds is 0x0181.
    LONGS_EQUAL(0x0080, virtualLeds);
}

TEST(LedDriver, UpperAndLowerBounds)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);
    LONGS_EQUAL(0x8001, virtualLeds);
}

IGNORE_TEST(LedDriver, TurnOnOutOfBoundsChangesNothing)
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);
    LONGS_EQUAL(0x0000, virtualLeds);
}

IGNORE_TEST(LedDriver, TurnOffOutOfBoundsChangesNothing)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);
    LONGS_EQUAL(0xffff, virtualLeds);
}

TEST(LedDriver, TurnOnOutOfBoundsThrowsRuntimeError)
{
    int ledNum = -1;
    LedDriver_TurnOn(ledNum);
    STRCMP_EQUAL("LED Driver: out-of-bounds LED",
                 RuntimeErrorStub_GetLastError());
    LONGS_EQUAL(ledNum, RuntimeErrorStub_GetLastParameter());
}

TEST(LedDriver, TurnOffOutOfBoundsThrowsRuntimeError)
{
    int ledNum = 17;
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(ledNum);
    STRCMP_EQUAL("LED Driver: out-of-bounds LED",
                 RuntimeErrorStub_GetLastError());
    LONGS_EQUAL(ledNum, RuntimeErrorStub_GetLastParameter());
}