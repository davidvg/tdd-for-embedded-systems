#ifndef _M28W160ECT_H
#define _M28W160ECT_H

typedef enum
{
    COMMAND_REGISTER = 0x0,
    STATUS_REGISTER = 0x0,
} Flash_Registers;

typedef enum
{
    PROGRAM_COMMAND = 0x40,
} Flash_Commands;

typedef enum
{
    ReadyBit = 1<<7,
} Status_Register_Bits;

#endif