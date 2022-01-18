#include "MockIO.h"
#include "CppUTest/TestHarness_c.h"

#include <stdio.h>

typedef struct Expectation
{
    int kind;
    ioAddress addr;
    ioData data;
} Expectation;

typedef enum
{
    NO_EXPECTED_VALUE = -1,
    FLASH_READ=0,
    FLASH_WRITE,
} Operation;

static Expectation *expectations = 0;
static int maxExpectationCount;
static int setExpectationCount;
static int getExpectationCount;

static Expectation expected;
static Expectation actual;

/*******************************************************************************
 * Messages
 ******************************************************************************/

// Failed expectations
static char * report_expected_read_was_write = 
    "Expected IO_Read(0x%x) would return 0x%x\n"
    "\t        But was IO_Write(0x%x, 0x%x)";

static char * report_expected_write_was_read = 
    "Expected IO_Write(0x%x, 0x%x)\n"
    "\t        But was IO_Read(0x%x)";

// Mismatched Write address or data
static char * report_write_does_not_match = 
    "Expected IO_Write(0x%x, 0x%x)\n"
    "\t        But was IO_Write(0x%x, 0x%x)";

static char * report_read_wrong_address = 
    "Expected IO_Read(0x%x) returns 0x%x\n"
    "\t        But was IO_Read(0x%x)";

// Other messages

static char * report_too_many_write_expectations = 
    "MockIO_Expect_Write: Too many expectations";

static char * report_too_many_read_expectations = 
    "MockIO_Expect_Read: Too many expectations";

static char * report_not_initialized = 
    "MockIO not initialized";

/*******************************************************************************
 * Failing methods
 ******************************************************************************/

/**
 * @brief Format a string for reporting an unmet expectation and fail with text 
 * 
 * @param msg 
 */
static void failExpectation(char * expectationFailMsg)
{
    char msg[100];
    int size = sizeof(msg) - 1;
    int offset;

    offset = snprintf(msg, size, expectationFailMsg,
                      expected.addr, expected.data,
                      actual.addr, actual.data);

    FAIL_TEXT_C(msg);
}

/**
 * @brief Checks if a failing condition is met and fails with a message
 * 
 * @param condition 
 * @param msg 
 */
static void failWhen(int condition, char *msg)
{
    if (condition)
    {
        failExpectation(msg);
    }
}

/**
 * @brief Checks that the Expectation's kind is NOT equal to the passed value
 * 
 * @param kind 
 * @return int 
 */
// static int expectationKindIsNot(int kind)
static int expectationKindDoesNotMatch(void)
{
    return expectations[getExpectationCount].kind != actual.kind;
}

/**
 * @brief Checks that the Expectation's address is NOT equal to the actual value
 * 
 * @return int 
 */
static int expectationAddressDoesNotMatch(void)
{
    return expected.addr != actual.addr;
}

static int expectationDataDoesNotMatch(void)
{
    return expected.data != actual.data;
}

static void failWhenTooManyExpectations(char *msg)
{
    if (setExpectationCount == maxExpectationCount)
        FAIL_TEXT_C(msg);
}

static void failWhenNotInitialized(void)
{
    if (!expectations)
        FAIL_TEXT_C(report_not_initialized);
}

/*******************************************************************************
 * Helping methods
 ******************************************************************************/

static void printExpectation(int number)
{
    Expectation exp = expectations[number];
    printf(">> Added: Exp=%d OP=%d ADDR=%d DATA=%d\n",
           (int)number, exp.kind, exp.addr, exp.data);
}

/**
 * @brief Adds a new Expectation to the expectations array
 * 
 * @param kind 
 * @param addr 
 * @param data 
 */
static void recordExpectation(Operation kind, ioAddress addr, ioData data)
{
    expectations[setExpectationCount].kind = kind;
    expectations[setExpectationCount].addr = addr;
    expectations[setExpectationCount].data = data;
    // printExpectation(setExpectationCount);
}

/**
 * @brief Set the expected Expectation to the next recorded Expectation and
 * @brief the actual Expectation to the intercepted Expectation
 * 
 * @param kind 
 * @param addr 
 * @param data 
 */
static void setExpectedAndActual(int kind, ioAddress addr, ioData data)
{
    expected.kind = expectations[getExpectationCount].kind;
    expected.addr = expectations[getExpectationCount].addr;
    expected.data = expectations[getExpectationCount].data;
    actual.kind = kind;
    actual.addr = addr;
    actual.data = data;
}

/*******************************************************************************
 * MockIO methods
 ******************************************************************************/
void MockIO_Create(int maxExpectations)
{
    maxExpectationCount = maxExpectations;
    expectations = calloc((size_t)maxExpectations, sizeof(Expectation));
    setExpectationCount = 0;
    getExpectationCount = 0;
}

void MockIO_Destroy(void)
{
    if (expectations)
        free(expectations);
    expectations = 0;

    maxExpectationCount = 0;
    setExpectationCount = 0;
    getExpectationCount = 0;
}

void MockIO_Expect_Write(ioAddress addr, ioData data)
{
    failWhenNotInitialized();
    failWhenTooManyExpectations(report_too_many_write_expectations);

    recordExpectation(FLASH_WRITE, addr, data);
    setExpectationCount++;
}

void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData data)
{
    failWhenNotInitialized();
    failWhenTooManyExpectations(report_too_many_read_expectations);

    recordExpectation(FLASH_READ, addr, data);
    setExpectationCount++;
}

/*******************************************************************************
 * Interface methods
 ******************************************************************************/

void IO_Write(ioAddress addr, ioData data)
{
    setExpectedAndActual(FLASH_WRITE, addr, data);

    failWhen(expectationKindDoesNotMatch(), report_expected_read_was_write);
    failWhen(expectationAddressDoesNotMatch(), report_write_does_not_match);
    failWhen(expectationDataDoesNotMatch(), report_write_does_not_match);

    getExpectationCount++;
}

ioData IO_Read(ioAddress addr)
{
    setExpectedAndActual(FLASH_READ, addr, NO_EXPECTED_VALUE);
    
    failWhen(expectationKindDoesNotMatch(), report_expected_write_was_read);
    failWhen(expectationAddressDoesNotMatch(), report_read_wrong_address);

    return expectations[getExpectationCount++].data;
}