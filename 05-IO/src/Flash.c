#include "Flash.h"


void Flash_Create(void)
{

}

void Flash_Destroy(void)
{

}

int Flash_Write(ioAddress addr, ioData data)
{
    IO_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    IO_Write(0x1000, 0xBEEF);
    IO_Read(STATUS_REGISTER);
    IO_Read(0x1000);
    return FLASH_SUCCESS;
}