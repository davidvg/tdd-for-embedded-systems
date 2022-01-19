#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"

extern "C"
{
    #include "MockIO.h"
}


static void MockIOSetup()
{
    MockIO_Create(2);
}

static void MockIOTeardown()
{
    MockIO_Destroy();
}

TEST_GROUP(MockIO)
{
    TestTestingFixture *fixture;
    int expectedErrors;

    void setup()
    {
        fixture = new TestTestingFixture();
        fixture->setSetup(MockIOSetup);
        fixture->setTeardown(MockIOTeardown);
        expectedErrors = 1;
    }

    void teardown()
    {
        delete fixture;
    }

    void testFailureWith(void (*method)())
    {
        fixture->setTestFunction(method);
        fixture->runAllTests();
        LONGS_EQUAL(expectedErrors, fixture->getFailureCount());
    }
};

/**
 * MockIO: CanMatchExpectations
 * - Create IO.h and declare IO_Write and IO_Read as explained in the book
 * - Create MockIO.h and MockIO.c and implement Create() and Destroy() with
 *   empty bodies
 * - Create MockIO_Expect_ReadThenReturn() and MockIO_Expect_Write(), empty
 * - MockIO.c: create IO_Write() and IO_Read() to override production code. It's
 *   not declared in MockIO.h. Here is where the steps are simulated, setting
 *   and getting values and variables
 */
static void CanMatchExpectations(void)
{
    MockIO_Expect_ReadThenReturn(1, 2);
    MockIO_Expect_Write(1, 2);
    ioData retval = IO_Read(1);
    IO_Write(1, retval);
}

TEST(MockIO, CanMatchExpectations)
{
    expectedErrors = 0;
    testFailureWith(CanMatchExpectations);
    fixture->assertPrintContains("OK");
}

/**
 * MockIO: WriteWhenReadExpectedFails
 * - MockIO.c: define static in maxExpectationCount and initialize it in Create()
 *   to the number of expectations passed as argument.
 * - MockIO.c: create struct Expectation with int kind, ioAddress addr and ioData
 *   data.
 * - Define a static Expectation expectations to hold an expectation
 * - MockIO.c: create an enum with possible operations: FLASH_READ, FLASH_WRITE
 * - ReadThenReturn(): update the expectations element
 * - IO_Write(): check that kind is WRITE and FAIL_TEXT_C with the correct
 *   message in other case.
 * 
 * [Test 1] To pass the test now I need to track more than one expectation.
 * - Declare expectations to be Expectation *expectations = 0
 * 
 * [Test 2] Adapt to new system
 * - MockIO.c: define an empty array of Expectation items, *expectations. In
 *   Create() allocate memory for holding the given number of structs using calloc.
 *   In Destroy() free the memory
 * - Create an static int setExpectationCount to keep track of the expectation
 *   to be set. In Create() initialize to 0
 * - Expect_ReadThenReturn(): add an Expectation to the array with index
 *   setExpectationCount and increment setExpectationCount
 * - MockIO_Expect_Write(): add an expectation and increment counter
 * - MockIO.c: Create static Expectation expected and static Expectation actual
 *   to hold expected and acutual values 
 * - MockIO.c: create static int getExpectationCount to keep track of the
 *   expectation to be used in IO_Write() and IO_Read() to set expected values
 * - IO_Write(): update expected with the contents of expectations[getExpCount]
 *   and actual with the arguments passed, and increment getExpCount
 * - IO_Read(): update expected and actual, and return data from
 *   expectations[getExpectationCount]
 * - IO_Write(): modify the if() to check expectations[getExpectationCount].kind
 */
static void ExpectedReadWasWriteFails()
{
    MockIO_Expect_ReadThenReturn(0, 1);
    IO_Write(0, 0);
}

TEST(MockIO, ExpectedReadWasWriteFails)
{
    testFailureWith(ExpectedReadWasWriteFails);
    fixture->assertPrintContains("Expected IO_Read(0x0) would return 0x1");
    fixture->assertPrintContains("But was IO_Write(0x0, 0x0)");
}

/**
 * MockIO: ExpectedWriteWasReadFails
 * - IO_Read(): check that the expectation's kind is correct like in the previous
 *   test
 * - Refactor to setExpectedKindIsNot(kind)
 * - Refactor failing messages
 * - Refactor failWhen(condition, msg)
 */
static void ExpectedWriteWasReadFails(void)
{
    MockIO_Expect_Write(0, 1);
    IO_Read(0);
}

TEST(MockIO, ExpectedWriteWasReadFails)
{
    testFailureWith(ExpectedWriteWasReadFails);
    fixture->assertPrintContains("Expected IO_Write(0x0, 0x1)");
    fixture->assertPrintContains("But was IO_Read(0x0)");
}

/**
 * MockIO: TooManyWriteExpectations
 * - Expect_Write(): check that setExpectationCount == maxExpectationCount and
 *   fail with text
 */
static void TooManyWriteExpectations(void)
{
    MockIO_Expect_Write(0, 1);
    MockIO_Expect_Write(0, 1);
    MockIO_Expect_Write(0, 1);
}

TEST(MockIO, TooManyWriteExpectations)
{
    testFailureWith(TooManyWriteExpectations);
    fixture->assertPrintContains("MockIO_Expect_Write: Too many expectations");
}

/**
 * MockIO: TooManyReadExpectations
 * - Expect_Read(): modify the method like in the previous test
 * - Refactor failing messages
 * - Refactor to failWhenTooManyExpectations(msg)
 */

static void TooManyReadExpectations()
{
    MockIO_Expect_ReadThenReturn(0, 0);
    MockIO_Expect_ReadThenReturn(0, 0);
    MockIO_Expect_ReadThenReturn(0, 0);
}

TEST(MockIO, TooManyReadExpectations)
{
  testFailureWith(TooManyReadExpectations);
  fixture->assertPrintContains("MockIO_Expect_Read: Too many expectations");
}

/**
 * MockIO: ExpectReadWhenNotInitialized
 * - Destroy(): check that expectations != 0 and then free it, and make it = 0
 * - In Expect_ReadThenReturn() check that expectations == 0 and fail with msg
 */
static void ExpectReadWhenNotInitialized(void)
{
    MockIO_Destroy();
    MockIO_Expect_ReadThenReturn(0, 0);
}

TEST(MockIO, ExpectReadWhenNotInitialized)
{
    testFailureWith(ExpectReadWhenNotInitialized);
    fixture->assertPrintContains("MockIO not initialized");
}

/**
 * MockIO: ExpectWriteWhenNotInitialized
 * - Modify Expect_Writee() like in the previous test
 * - Refactor to failWhenNotInitialized()
 */
static void ExpectWriteWhenNotInitialized(void)
{
    MockIO_Destroy();
    MockIO_Expect_Write(0, 0);
}

TEST(MockIO, ExpectWriteWhenNotInitialized)
{
    testFailureWith(ExpectWriteWhenNotInitialized);
    fixture->assertPrintContains("MockIO not initialized");
}

/**
 * MockIO: MismatchedWriteAddress
 * - IO_Write(): check expected.addr != actual.addr and fail with msg
 */
static void MismatchedWriteAddress(void)
{
    MockIO_Expect_Write(0, 0);
    IO_Write(0x10, 0);
}

TEST(MockIO, MismatchedWriteAddress)
{
    testFailureWith(MismatchedWriteAddress);
    fixture->assertPrintContains("Expected IO_Write(0x0, 0x0)");
    fixture->assertPrintContains("But was IO_Write(0x10, 0x0)");
}

/**
 * MockIO: MismatchedReadAddress
 * - IO_Read(): modify it like in the previous test
 * - Refactor to expectationAddressIsNot(addr)
 */
static void MismatchedReadAddress()
{
    MockIO_Expect_ReadThenReturn(0x1000, 0xaaaa);
    IO_Read(0x10000);
}

TEST(MockIO, MismatchedReadAddress)
{
  testFailureWith(MismatchedReadAddress);
  fixture->assertPrintContains("Expected IO_Read(0x1000) returns 0xaaaa");
  fixture->assertPrintContains("But was IO_Read(0x10000)");
}

/**
 * MockIO, MismatchedWriteData
 * - Check that expected.data != actual.data and failExpectation() with the
 *   same message than the address failing
 * - Refactor the check into expectationDataDoesNotMatch()+
 * - Modify the code to use failWhen()
 */
static void MismatchedWriteData(void)
{
    MockIO_Expect_Write(0, 0);
    IO_Write(0, 0xDEAD);
}

TEST(MockIO, MismatchedWriteData)
{
    testFailureWith(MismatchedWriteData);
    fixture->assertPrintContains("Expected IO_Write(0x0, 0x0)");
    fixture->assertPrintContains("But was IO_Write(0x0, 0xdead)");
}

/**
 * MockIO: TooManyReads
 * - IO_Read(): check if setExpCount == getExpCount and fail with message
 * - Refactor to use a new method failWhenNoUnusedExpectations(msg)
 * - Inside this method, format the string to include values from expectations
 */
static void TooManyReads(void)
{
    IO_Read(0x1000);
}

TEST(MockIO, TooManyReads)
{
    testFailureWith(TooManyReads);
    fixture->assertPrintContains("No more expectations but was IO_Read(0x1000)");
}

/**
 * MockIO: TooManyWrites
 * - Use failWhenNoUnusedExpectations() with a new message for write()
 * - Rename report_read_no_unused_expectations to report_read_but_no_unused_expectations
 *   to allow different messages for R/W when using this failure method
 * - Add a new fail message for Write with address and data specifiers
 * - In failWhenNoUnusedExpectations(), in snprintf(), pass a new value to format
 *   the string. It will be used when 2 specifiers are used (IO_Write) and only
 *   the first one when only one is needed (IO_Read)
 */
static void TooManyWrites(void)
{
    IO_Write(0x1000, 0x1234);
}

TEST(MockIO, TooManyWrites)
{
    testFailureWith(TooManyWrites);
    fixture->assertPrintContains("No more expectations but was IO_Write(0x1000, 0x1234)");
}

/**
 * MockIO, ReadWhenNoUnusedExpectationsWithRW
 * - Create a new failure string report_no_more_expectations with content:
 *   "R/W: %d: No more expectations but was "
 * - Modify report_read_but_no_unused_expectations to remove the duplicated
 *   string
 * - Modify failWhenNoUnusedExpectations() to use snprintf to include getExpCount+1
 *   in the created string (report_no_more_expectations)
 * - Use snprintf to include expectation values using the Read failing report.
 */
static void ReadWhenNoUnusedExpectationsWithRW(void)
{
    IO_Read(0x10);
}

TEST(MockIO, ReadWhenNoUnusedExpectationsWithRW)
{
    testFailureWith(ReadWhenNoUnusedExpectationsWithRW);
    fixture->assertPrintContains("R/W 1:");
}

/**
 * MockIO, WriteWhenNoUnusedExpectationsWithRW
 * - Modify report_write_but_no_unused_expectations like in the previous test
 */
static void WriteWhenNoUnusedExpectationsWithRW(void)
{
    IO_Write(0x100, 0x4321);
}

TEST(MockIO, WriteWhenNoUnusedExpectationsWithRW)
{
    testFailureWith(WriteWhenNoUnusedExpectationsWithRW);
    fixture->assertPrintContains("R/W 1:");
}

/**
 * MockIO: Wrong Address uses R/W
 * 
 * FailExpectation() is called inside failWhen(). This happens when:
 *   - Kind does not match
 *   - Address does not match
 *   - Data doese not match
 * 
 * - Create a new string, report_expectation_number, with "R/W %d: "
 * - Inside failExpectation() use sprintf() to include expectation number
 * - Use sprintf() to format expectation values
 */
static void ReadWrongAddressFailsWithRW(void)
{
    MockIO_Expect_ReadThenReturn(0x100, 0x1234);
    
    IO_Read(0x10);
}

TEST(MockIO, ReadWrongAddressFailsWithRW)
{
    testFailureWith(ReadWrongAddressFailsWithRW);
    fixture->assertPrintContains("R/W 1:");
}


static void WriteWrongAddressFailsWithRW(void)
{
    MockIO_Expect_Write(0x100, 0x1234);
    
    IO_Write(0x1, 0x1234);
}

TEST(MockIO, WriteWrongAddressFailsWithRW)
{
    testFailureWith(WriteWrongAddressFailsWithRW);
    fixture->assertPrintContains("R/W 1:");
}

/**
 * Wrong Kind
 */
static void WriteWhenExpectedReadFailsWithRW(void)
{
    MockIO_Expect_ReadThenReturn(0x1, 0x1);
    IO_Write(0x1, 0x1);
}

TEST(MockIO, WriteWhenExpectedReadFailsWithRW)
{
    testFailureWith(WriteWhenExpectedReadFailsWithRW);
    fixture->assertPrintContains("R/W 1:");
}


static void ReadWhenExpectedWriteFailsWithRW(void)
{
    MockIO_Expect_Write(0x1, 0x1);
    IO_Read(0x1);
}

TEST(MockIO, ReadWhenExpectedWriteFailsWithRW)
{
    testFailureWith(ReadWhenExpectedWriteFailsWithRW);
    fixture->assertPrintContains("R/W 1:");
}

/**
 * Wrong data
 */
static void WriteWrongDataFailsWithRW(void)
{
    MockIO_Expect_Write(0x1, 0x2);
    IO_Write(0x1, 0xDEAD);
}

TEST(MockIO, WriteWrongDataFailsWithRW)
{
    testFailureWith(WriteWrongDataFailsWithRW);
    fixture->assertPrintContains("R/W 1: ");
}

/**
 * MockIO: NotAllExpectationsUsed
 * - Declare a new function MockIO_VerifyComplete(void) in the header file
 * - Implement it to check if getExpcCont != MaxExpCount and fail with message
 * - Create a new fail string, report_not_all_expectations_used
 * - Create a new fail method, failWhenNotAllExpectationsUsed() and move the 
 *   check inside
 * - Modify the string to include specifiers to data
 * - Modify the method to use sprintf and update the string using maxExpCount
 *   and getExpCount
 */
static void NotAllExpectationsUsed(void)
{
    MockIO_Expect_ReadThenReturn(0x1000, 0xaaaa);
    MockIO_Expect_Write(0x2000, 0x5555);

    MockIO_VerifyComplete();
}

TEST(MockIO, NotAllExpectationsUsed)
{
    testFailureWith(NotAllExpectationsUsed);
    fixture->assertPrintContains("Expected 2 reads/writes but got 0");
}