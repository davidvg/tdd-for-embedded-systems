#include "unity.h"
#include "LedDriver.h"

#include "RuntimeErrorStub.h"

static uint16_t virtualLeds;


void setUp() {
    LedDriver_Create(&virtualLeds);
}

void tearDown() {}

void test_all_leds_off_after_create(void)
{
    // Special case: instantiation is done here
    uint16_t virtualLeds = 0xffff;
    LedDriver_Create(&virtualLeds);
    TEST_ASSERT_EQUAL_HEX16(0x0, virtualLeds);
}

void test_turn_on_led_one(void)
{
    LedDriver_TurnOn(1);
    TEST_ASSERT_EQUAL_HEX16(0x01, virtualLeds);
}

void test_turn_off_led_one(void)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);
    TEST_ASSERT_EQUAL_HEX16(0x0, virtualLeds);
}

void test_turn_all_on(void) {
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(8);
    TEST_ASSERT_EQUAL_HEX16(0xff7f, virtualLeds);
}

void test_turn_off_any_led(void)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(8);
    TEST_ASSERT_EQUAL_HEX16(0xff7f, virtualLeds);
}

void test_turn_all_off(void)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnAllOff();
    TEST_ASSERT_EQUAL_HEX16(0x00, virtualLeds);
}

/**
 * The datasheet for the Led Driver says that the status register for the
 * driver is not readable. This means that if we want to obtain the current
 * status for the LED bank, we can't get it from the driver.
 * 
 * The following test checks that the current state is obtained. 
 * 
 * After LedDriver_Create(&virtualLeds), virtualLeds is 0x000.
 * virtualLeds is passed by address, so it gets updated when modifications
 * are done to the LED register in LedDriver.c
 * If it's overwritten, this update does not happen anymore.
 */
void test_led_memory_is_not_readable(void)
{
    // After calling LedDriver_Create(&virtualLeds), virtualLeds=0x0000.
    // Overwrite virtualLeds with a value whose 8-th bit is 0.
    virtualLeds = 0x0101;
    // When LedDriver_TurnOn(8) is called, a 1 is written to the 8-th bit,
    // and the remaining LEDs are off, so virtualLeds should be 0x0080. If 
    // the state was readable, this would be the value of virtualLeds after
    // turning the LED on.
    // LedDriver_TurnOn(8);
    LedDriver_TurnOff(1);
    // As the state is not readable, the 8-th bit has been set and the current
    // value for virtualLeds is 0x0181
    // TEST_ASSERT_EQUAL_HEX16(0x0080, virtualLeds);
    TEST_ASSERT_EQUAL_HEX16(0x0000, virtualLeds);
}

void test_turn_on_out_of_bounds_do_nothing(void)
{
    TEST_IGNORE();
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);
    TEST_ASSERT_EQUAL_HEX16(0x00, virtualLeds);
}

void test_turn_off_out_of_bounds_do_nothing(void)
{
    TEST_IGNORE();
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);
    TEST_ASSERT_EQUAL_HEX16(0xffff, virtualLeds);
}

// TODO Test: check out-of-bounds values

void test_turn_on_out_of_bounds_throws_runtime_error(void)
{
    int ledNum = 17;
    LedDriver_TurnOn(ledNum);
    TEST_ASSERT_EQUAL_STRING("LED Driver: out-of-bounds LED",
                             RuntimeErrorStub_GetLastError());
    TEST_ASSERT_EQUAL(ledNum, RuntimeErrorStub_GetLastParameter());
}

void test_turn_off_out_of_bounds_throws_runtime_error(void)
{
    int ledNum = -1;
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(ledNum);
    TEST_ASSERT_EQUAL_STRING("LED Driver: out-of-bounds LED",
                             RuntimeErrorStub_GetLastError());
    TEST_ASSERT_EQUAL(ledNum, RuntimeErrorStub_GetLastParameter());
}

//------------------------------------------------------------------------------
int main(void) 
{
    UNITY_BEGIN();
    RUN_TEST(test_all_leds_off_after_create);
    RUN_TEST(test_turn_on_led_one);
    RUN_TEST(test_turn_off_led_one);
    RUN_TEST(test_turn_all_on);
    RUN_TEST(test_turn_off_any_led);
    RUN_TEST(test_turn_all_off);
    RUN_TEST(test_led_memory_is_not_readable);
    RUN_TEST(test_turn_on_out_of_bounds_do_nothing);
    RUN_TEST(test_turn_off_out_of_bounds_do_nothing);
    RUN_TEST(test_turn_on_out_of_bounds_throws_runtime_error);
    RUN_TEST(test_turn_off_out_of_bounds_throws_runtime_error);

    return UNITY_END();
}