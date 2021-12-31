#include "LedDriver.h"
#include "RuntimeError.h"
#include <stdbool.h>

static uint16_t *ledsAddress;    // Led register address
/**
 * The state register is not readable, so we need an intermediate variable to
 * store the LEDs state. This acts as a safe in case the state variable is
 * modified and needs to be recovered, and as the register is write-only,
 * the actual value can be obtained from this intermediate variable. * 
 */
static uint16_t ledsImage;      // Intermediate variable for state

enum
{
    ALL_LEDS_ON = ~0,
    ALL_LEDS_OFF = ~ALL_LEDS_ON
};

enum
{
    MIN_LED_NUM=1,
    MAX_LED_NUM=16
};

static uint16_t convertLedNumberToBit(int ledNumber)
{
    return 1 << (ledNumber - 1);
}

static void updateHardware()
{
    *ledsAddress = ledsImage;
}

static bool isLedOutOfBounds(int ledNumber)
{
    return (ledNumber < MIN_LED_NUM || ledNumber > MAX_LED_NUM);
}

static void setLedImageBit(int ledNumber)
{
    ledsImage |= convertLedNumberToBit(ledNumber);
}

static void clearLedImageBit(int ledNumber)
{
    ledsImage &= ~convertLedNumberToBit(ledNumber);
}

// *** INTERFACE *** //

void LedDriver_Create(uint16_t *address)
{
    ledsAddress = address;
    // The state is stored in an intermediate variable, so if the LED state
    // *ledsAddress is modified from outside the code, the actual value is
    // safe here
    ledsImage = ALL_LEDS_OFF;
    // The state variable is updated from the intermediate variable
    updateHardware();
}

void LedDriver_TurnOn(int ledNumber)
{
    if (isLedOutOfBounds(ledNumber)) {
        RUNTIME_ERROR("LED Driver: out-of-bounds LED", ledNumber);
        return;
    }

    setLedImageBit(ledNumber);
    updateHardware();
}

void LedDriver_TurnOff(int ledNumber)
{
    if (isLedOutOfBounds(ledNumber)) {
        RUNTIME_ERROR("LED Driver: out-of-bounds LED", ledNumber);
        return;
    }

    clearLedImageBit(ledNumber);
    updateHardware();
}

void LedDriver_TurnAllOn(void)
{
    ledsImage = ALL_LEDS_ON;
    updateHardware();
}

void LedDriver_TurnAllOff(void)
{
    ledsImage = ALL_LEDS_OFF;
    updateHardware();
}

bool LedDriver_IsOn(int ledNumber)
{
    if (isLedOutOfBounds(ledNumber))
        return false;

    return ledsImage & (convertLedNumberToBit(ledNumber));
}

bool LedDriver_IsOff(int ledNumber)
{
    if (isLedOutOfBounds(ledNumber))
        return true;

    return !LedDriver_IsOn(ledNumber);
}