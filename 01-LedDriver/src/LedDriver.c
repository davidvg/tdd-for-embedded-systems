#include "LedDriver.h"

static uint16_t *ledsAddress;
/**
 * According to the datasheet, the LED register is not readable, so the
 * current LED status is stored in a private variable. When LEDs are turn on
 * or off, the state is stored here and any status query from the client is
 * responded using this variable instead of the write-only register.
 */
static uint16_t ledsImage;

// TODO Check casting
enum {
    ALL_LEDS_ON = 0xffff,
    ALL_LEDS_OFF = (uint16_t)~ALL_LEDS_ON
};

enum
{
    MIN_LED_NUM=1,
    MAX_LED_NUM=16
};


static uint16_t convertLedNumberToBit(int ledNumber)
{
    // TODO Check casting
    return (uint16_t)(1 << (ledNumber-1));
}

static void updateHardware(void)
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
    // TODO Check casting
    ledsImage &= (uint16_t)~(convertLedNumberToBit(ledNumber));
}

/* Interface */

void LedDriver_Create(uint16_t *address)
{
    ledsAddress = address;
    // TODO Check why casting from int to uint16_t throws a warning
    ledsImage = (uint16_t)ALL_LEDS_OFF;
    updateHardware();
}

void LedDriver_Destroy(void)
{}

void LedDriver_TurnOn(int ledNumber)
{
    if (isLedOutOfBounds(ledNumber))
    {
        RUNTIME_ERROR("LED Driver: out-of-bounds LED", ledNumber);
        return;
    }

    setLedImageBit(ledNumber);
    updateHardware();
}

void LedDriver_TurnOff(int ledNumber)
{
    if (isLedOutOfBounds(ledNumber))
    {
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
    return ledsImage & convertLedNumberToBit(ledNumber);
}

bool LedDriver_IsOff(int ledNumber)
{
    if (isLedOutOfBounds(ledNumber))
        return true;
    return !LedDriver_IsOn(ledNumber);    
}