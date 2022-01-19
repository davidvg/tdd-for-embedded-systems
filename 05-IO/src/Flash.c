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

    if (status != ReadyBit)
    {
        if (status & VppErrorBit)
            return FLASH_VPP_ERROR;
        if (status & ProgramErrorBit)
            return FLASH_PROGRAM_ERROR;
        if (status & ProtectedBlockBit)
            return FLASH_PROTECTED_BLOCK_ERROR;
    }
    IO_Read(addr);
    return FLASH_SUCCESS;
}