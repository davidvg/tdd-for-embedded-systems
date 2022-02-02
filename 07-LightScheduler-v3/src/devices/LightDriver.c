#include "LightDriver.h"


void LightDriver_Destroy(LightDriver driver)
{
    free(driver);
}