#ifndef _FLASH_H
#define _FLASH_H

#include "IO.h"
#include "m28w160ect.h"

typedef enum
{
    FLASH_SUCCESS=0,
    FLASH_VPP_ERROR,
    FLASH_PROGRAM_ERROR,
    FLASH_PROTECTED_BLOCK_ERROR,
    FLASH_READ_BACK_ERROR,
} Flash_Status;


void Flash_Create(void);
void Flash_Destroy(void);
int Flash_Write(ioAddress addr, ioData data);



#endif