#include "CppUTest/TestHarness.h"

extern "C"
{
    #include "Flash.h"
    #include "MockIO.h"
}

ioAddress address;
ioData data;


TEST_GROUP(Flash)
{
    void setup()
    {
        address = 0x1000;
        data = 0xBEEF;

        MockIO_Create(10);
        Flash_Create();
    }

    void teardown()
    {
        Flash_Destroy();
        MockIO_Destroy();
        MockIO_VerifyComplete();
    }

};

/**
 * Flash: WriteSucceeds_ReadyInmediately
 * - Declare Flash_Create() and Flash_Destroy()
 * - Declare Flash_Write(ioAddress, ioData) and implement it to make the calls
 *   to IO_Write() and IO_Read() with the specific values
 * - Create an enum, Flash_Status, with FLASH_SUCCESS=0
 * - Return FLASH_SUCCESS
 * - Refactor: add address and data to setup()
 * - Refactor: create m28w160ect.h and add CommandRegister and StatusRegister
 *   in an enum Flash_Registers. Modify code to use them.
 * - Refactor: In the previous header file create another enum, Flash_Commands,
 *   with the ProgramCommand
 * - Refactor: create a new enum in the same header, Status_Register_Bits,
 *   and add ReadyBit = 1<<7;
 */
TEST(Flash, WriteSucceeds_ReadyInmediately)
{
    MockIO_Expect_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, ReadyBit);
    MockIO_Expect_ReadThenReturn(address, data);
    
    int result = Flash_Write(address, data);

    LONGS_EQUAL(0, result);
    MockIO_VerifyComplete(); 
}