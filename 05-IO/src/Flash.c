#include "Flash.h"


void Flash_Create(void)
{

}

void Flash_Destroy(void)
{

}

int Flash_Write(ioAddress addr, ioData data)
{
    ioData status = 0;

    IO_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    IO_Write(addr, data);

    while((status & ReadyBit) == 0)
    {
        status = IO_Read(STATUS_REGISTER);
    }

    IO_Read(addr);
    return FLASH_SUCCESS;
}