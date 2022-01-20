#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
    #include "CppUTestExt/MockSupport_c.h"
    #include "Flash.h"
    #include "m28w160ect.h"
    #include "FakeMicroTime.h"
}

/**
 * Mock objects for IO_Write() and IO_Read()
 */
void IO_Write(ioAddress addr, ioData data)
{
    mock_c()->actualCall("IO_Write")
            ->withIntParameters("addr", addr)
            ->withIntParameters("data", data);
}

ioData IO_Read(ioAddress addr)
{
    mock_c()->actualCall("IO_Read")
            ->withIntParameters("addr", addr);
    return mock_c()->returnValue().value.intValue;
}

//------------------------------------------------------------------------------

ioAddress address;
ioData value;
int retval;


TEST_GROUP(FlashCppUMock)
{
    void setup()
    {
        address = 0xfeed;
        value = 0x1dea;
        Flash_Create();
    }

    void teardown()
    {
        Flash_Destroy();
        mock().checkExpectations();
        mock().clear();
    }  

    /**
     * Helper functions 
     */

    void expectCommand(Flash_Commands command)
    {
        mock().expectOneCall("IO_Write")
            .withParameter("addr", COMMAND_REGISTER)
            .withParameter("data", command);
    }

    void expectWriteData(ioAddress addr, ioData data)
    {
        mock().expectOneCall("IO_Write")
            .withParameter("addr", addr)
            .withParameter("data", data);
    }

    void expectReadStatusAndReturn(ioData status)
    {
        mock().expectOneCall("IO_Read")
            .withParameter("addr", STATUS_REGISTER)
            .andReturnValue(status);
    }

    void expectReadAddressAndReturn(ioAddress addr, ioData data)
    {
        mock().expectOneCall("IO_Read")
            .withParameter("addr", addr)
            .andReturnValue(data);
    }

    void expectReadStatusAndReturnWithRepeat(ioData status, uint repeatCount)
    {
        mock().expectNCalls(repeatCount, "IO_Read")
            .withParameter("addr", STATUS_REGISTER)
            .andReturnValue(status);
    }
};

/**
 * Tests
 */

TEST(FlashCppUMock, WriteSucceeds_Immediately)
{
    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturn(ReadyBit);
    expectReadAddressAndReturn(address, value);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_SUCCESS, retval);
}

TEST(FlashCppUMock, WriteSucceeds_NotInmediatelyReady)
{
    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturnWithRepeat(0, 3);
    expectReadStatusAndReturn(ReadyBit);
    expectReadAddressAndReturn(address, value);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_SUCCESS, retval);
}

TEST(FlashCppUMock, WriteFalls_VppError)
{
    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturn(ReadyBit | VppErrorBit);
    expectCommand(RESET_COMMAND);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_VPP_ERROR, retval);
}

TEST(FlashCppUMock, WriteFalls_ProgramError)
{
    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturn(ReadyBit | ProgramErrorBit);
    expectCommand(RESET_COMMAND);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_PROGRAM_ERROR, retval);
}

TEST(FlashCppUMock, WriteFalls_ProtectedBlockError)
{
    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturn(ReadyBit | ProtectedBlockBit);
    expectCommand(RESET_COMMAND);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_PROTECTED_BLOCK_ERROR, retval);
}

TEST(FlashCppUMock, WriteFails_FlashReadBackError)
{
    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturn(ReadyBit);
    expectReadAddressAndReturn(address, value-1);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_READ_BACK_ERROR, retval);
}

TEST(FlashCppUMock, WriteSucceeds_IgnoresOtherBitsUntilReady)
{
    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturn(~ReadyBit);
    expectReadStatusAndReturn(ReadyBit);
    expectReadAddressAndReturn(address, value);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_SUCCESS, retval);
}

TEST(FlashCppUMock, WriteFails_Timeout)
{
    FakeMicroTime_Init(0, 500);
    Flash_Create();

    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturnWithRepeat(~ReadyBit, 9);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_TIMEOUT_ERROR, retval);
}

TEST(FlashCppUMock, WriteFails_TimerRollsOver)
{
    FakeMicroTime_Init(0xFFFFFFFF, 500);
    Flash_Create();

    expectCommand(PROGRAM_COMMAND);
    expectWriteData(address, value);
    expectReadStatusAndReturnWithRepeat(~ReadyBit, 9);

    retval = Flash_Write(address, value);
    LONGS_EQUAL(FLASH_TIMEOUT_ERROR, retval);
}