#include "CppUTest/TestHarness.h"

extern "C"
{
    #include "Flash.h"
    #include "MockIO.h"
}

ioAddress address;
ioData data;

int result;


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
    
    result = Flash_Write(address, data);

    LONGS_EQUAL(0, result);
}

/**
 * Flash: WriteSucceeds_NotInmediatelyReady
 * - Create a ioData status=0 inside F_Write()
 * - Crate a while to loop while (status & ReadyBit) = 0; inside the loop
 *   update status = IO_Read(STATUS_REGISTER)
 */
TEST(Flash, WriteSucceeds_NotInmediatelyReady)
{
    MockIO_Expect_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, 0);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, 0);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, 0);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, ReadyBit);
    MockIO_Expect_ReadThenReturn(address, data);

    result = Flash_Write(address, data);
    LONGS_EQUAL(0, result);
}

/**
 * Flash: WriteFalls_VppError
 * - Define VppErrorBit=1<<3 in the enum, in the device header file.
 * - Define the command RESET=0xFF in the enum
 * - Define FLASH_VPP_ERROR in the Flash_Status enum
 * - After reading STATUS_REGISTER, check if (status & VppErrorBit == True) and
 *   then return the error
 */
TEST(Flash, WriteFalls_VppError)
{
    MockIO_Expect_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, ReadyBit | VppErrorBit);
    MockIO_Expect_Write(COMMAND_REGISTER, RESET_COMMAND);

    result = Flash_Write(address, data);

    LONGS_EQUAL(FLASH_VPP_ERROR, result);
}

/**
 * Flash: WriteFalls_ProgramError
 * - Fix the code as in the previous test
 */
TEST(Flash, WriteFalls_ProgramError)
{
    MockIO_Expect_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, ReadyBit | ProgramErrorBit);
    MockIO_Expect_Write(COMMAND_REGISTER, RESET_COMMAND);

    result = Flash_Write(address, data);

    LONGS_EQUAL(FLASH_PROGRAM_ERROR, result);
}

/**
 * Flash: WriteFalls_ProtectedBlockError
 * - Fix the code as in the previous test
 */
TEST(Flash, WriteFalls_ProtectedBlockError)
{
    MockIO_Expect_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, ReadyBit | ProtectedBlockBit);
    MockIO_Expect_Write(COMMAND_REGISTER, RESET_COMMAND);

    result = Flash_Write(address, data);

    LONGS_EQUAL(FLASH_PROTECTED_BLOCK_ERROR, result);
}

/**
 * Flash: WriteFails_FlashReadBackError
 * After writing, a read back is done. This test checks that the correct error
 * is returned when read data does not match written data
 * - Add the new error type
 * - After error checking, check that IO_Read() match data, and return the erro
 *   if it doesn't
 */
TEST(Flash, WriteFails_FlashReadBackError)
{
    MockIO_Expect_Write(COMMAND_REGISTER, PROGRAM_COMMAND);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(STATUS_REGISTER, ReadyBit);
    MockIO_Expect_ReadThenReturn(address, data-1);

    result = Flash_Write(address, data);

    LONGS_EQUAL(FLASH_READ_BACK_ERROR, result);
}