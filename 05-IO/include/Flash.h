#ifndef _FLASH_H
#define _FLASH_H

#include "IO.h"
#include "m28w160ect.h"

typedef enum
{
    FLASH_SUCCESS=0,
} Flash_Status;


void Flash_Create(void);
void Flash_Destroy(void);
int Flash_Write(ioAddress addr, ioData data);






#endif