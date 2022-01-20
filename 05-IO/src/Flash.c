#include "Flash.h"

#define FLASH_WRITE_TIMEOUT_MICROSECONDS    5000


void Flash_Create(void)
{

}

void Flash_Destroy(void)
{

}

static int writeError(ioData status)
{
    IO_Write(COMMAND_REGISTER, RESET_COMMAND);
    
    if (status & VppErrorBit)
        return FLASH_VPP_ERROR;
    else if (status & ProgramErrorBit)
        return FLASH_PROGRAM_ERROR;
    else if (status & ProtectedBlockBit)
        return FLASH_PROTECTED_BLOCK_ERROR;
    else
        return FLASH_UNKNOWN_ERROR;
}

int Flash_Write(ioAddress addr, ioData data)
{
    ioData status = 0;
    uint32_t timestamp = MicroTime_Get();

    IO_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    IO_Write(addr, data);


    while((status & ReadyBit) == 0)
    {         
        if (MicroTime_Get() - timestamp >= FLASH_WRITE_TIMEOUT_MICROSECONDS)
            return FLASH_TIMEOUT_ERROR;

        status = IO_Read(STATUS_REGISTER);
    }

    if (status != ReadyBit)
        return writeError(status);

    if (data != IO_Read(addr))
        return FLASH_READ_BACK_ERROR;

    return FLASH_SUCCESS;
}