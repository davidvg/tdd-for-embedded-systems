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

static int failureAlreadyReported;

/*******************************************************************************
 * Messages
 ******************************************************************************/

// Failed expectations
static char * report_expected_read_was_write = 
    "Expected IO_Read(0x%x) would return 0x%x\n"
    "\t       But was IO_Write(0x%x, 0x%x)";

static char * report_expected_write_was_read = 
    "Expected IO_Write(0x%x, 0x%x)\n"
    "\t       But was IO_Read(0x%x)";

// Mismatched Write address or data
static char * report_write_does_not_match = 
    "Expected IO_Write(0x%x, 0x%x)\n"
    "\t       But was IO_Write(0x%x, 0x%x)";

static char * report_read_wrong_address = 
    "Expected IO_Read(0x%x) returns 0x%x\n"
    "\t       But was IO_Read(0x%x)";

// Other messages
static char * report_not_initialized = 
    "MockIO not initialized";

static char * report_too_many_write_expectations = 
    "MockIO_Expect_Write: Too many expectations";

static char * report_too_many_read_expectations = 
    "MockIO_Expect_Read: Too many expectations";

static char * report_no_more_expectations =
    "R/W %d: No more expectations but was ";

static char * report_read_but_no_unused_expectations = 
    "IO_Read(0x%x)";

static char * report_write_but_no_unused_expectations = 
    "IO_Write(0x%x, 0x%x)";

static char * report_expectation_number = 
    "R/W %d: ";

static char * report_not_all_expectations_used =
    "Expected %d reads/writes but got %d";

/*******************************************************************************
 * Checks
 ******************************************************************************/

/**
 * @brief Checks that the Expectation's kind is NOT equal to the passed value
 * 
 * @return int 
 */
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

/**
 * @brief Checks that the expected data is correct
 * 
 * @return int 
 */
static int expectationDataDoesNotMatch(void)
{
    return expected.data != actual.data;
}

/**
 * @brief Checks that there are no unused expectations
 * 
 * @return int 
 */
static int noUnusedExpectations(void)
{
    return getExpectationCount == setExpectationCount;
}

/*******************************************************************************
 * Failing methods
 ******************************************************************************/

/**
 * @brief Updates failureAlreadyReported=1 and fails with the passed message
 * 
 * @param failingMsg 
 */
static void fail(char * failingMsg)
{
    failureAlreadyReported = 1;
    FAIL_TEXT_C(failingMsg);
}

/**
 * @brief Checks that the expectations array has been initialized
 * 
 */
static void failWhenNotInitialized(void)
{
    if (!expectations)
        fail(report_not_initialized);
}

/**
 * @brief Format a string for reporting an unmet expectation and fail with text 
 *
 * @param expectationFailMsg 
 */
static void failExpectation(char * expectationFailMsg)
{
    char msg[100];
    size_t size = sizeof(msg) - 1;
    int offset;

    offset = snprintf(msg, size, report_expectation_number, getExpectationCount+1);

    offset = snprintf(msg+offset, size-(size_t)offset, expectationFailMsg,
                      expected.addr, expected.data,
                      actual.addr, actual.data);

    fail(msg);
}

/**
 * @brief Checks if a failing condition is met and fails with a message
 * 
 * @param condition 
 * @param expectationFailMsg 
 */
static void failWhen(int condition, char *expectationFailMsg)
{
    if (condition)
    {
        failExpectation(expectationFailMsg);
    }
}

/**
 * @brief Checks that there the maximum number of expectations have been recorded
 * @brief and a new one cannot be added to the list, and fails with a message
 * 
 * @param expectationFailMsg 
 */
static void failWhenTooManyExpectations(char *expectationFailMsg)
{
    if (setExpectationCount == maxExpectationCount)
        fail(expectationFailMsg);
}

/**
 * @brief Checks that there are no unused expectations and fails with a message
 * 
 * @param expectationFailMsg 
 */
static void failWhenNoUnusedExpectations(char * expectationFailMsg)
{
    if (noUnusedExpectations())
    {
        char msg[100];
        size_t size = sizeof(msg) - 1;
        int offset;

        offset = snprintf(msg, size, report_no_more_expectations,
                          getExpectationCount + 1);

        offset = snprintf(msg+offset, size-(size_t)offset, expectationFailMsg,
                          actual.addr, actual.data);

        fail(msg);
    }
}

/**
 * @brief Checks that there are no unused expectations and fails with message
 * 
 */
static void failWhenNotAllExpectationsUsed(void)
{
    if (getExpectationCount == setExpectationCount)
        return;
        
    char msg[100];
    size_t size = sizeof(msg) - 1;

    snprintf(msg, size, report_not_all_expectations_used,
                setExpectationCount, getExpectationCount);

    fail(msg);
    
}

/*******************************************************************************
 * Helping methods
 ******************************************************************************/

/**
 * @brief Prints a formatted string with the contents of a numbered Expectation
 * 
 * @param number 
 */
// static void printExpectation(int number)
// {
//     Expectation exp = expectations[number];
//     printf(">> Added: Exp=%d OP=%d ADDR=%d DATA=%d\n",
//            (int)number, exp.kind, exp.addr, exp.data);
// }

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

/**
 * @brief Initializes the Mock and its components
 * 
 * @param maxExpectations 
 */
void MockIO_Create(int maxExpectations)
{
    maxExpectationCount = maxExpectations;
    expectations = calloc((size_t)maxExpectations, sizeof(Expectation));
    setExpectationCount = 0;
    getExpectationCount = 0;
    failureAlreadyReported = 0;
}

/**
 * @brief Destroys the Mock and resets its components
 * 
 */
void MockIO_Destroy(void)
{
    if (expectations)
        free(expectations);
    expectations = 0;

    maxExpectationCount = 0;
    setExpectationCount = 0;
    getExpectationCount = 0;
    failureAlreadyReported = 0;
}

/**
 * @brief Add a new IO_Write() Expectation
 * 
 * @param addr 
 * @param data 
 */
void MockIO_Expect_Write(ioAddress addr, ioData data)
{
    failWhenNotInitialized();
    failWhenTooManyExpectations(report_too_many_write_expectations);

    recordExpectation(FLASH_WRITE, addr, data);
    setExpectationCount++;
}

/**
 * @brief Add a new IO_Read() Expectation
 * 
 * @param addr 
 * @param data 
 */
void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData data)
{
    failWhenNotInitialized();
    failWhenTooManyExpectations(report_too_many_read_expectations);

    recordExpectation(FLASH_READ, addr, data);
    setExpectationCount++;
}

/**
 * @brief Checks that all the recorded expectations have been used. If a failure
 * @brief has already been reported inside fail(), skips the check to avoid
 * @brief wrong behaviour
 * 
 */
void MockIO_VerifyComplete(void)
{
    if (failureAlreadyReported)
        return;
    failWhenNotAllExpectationsUsed();
}

/*******************************************************************************
 * Interface methods
 ******************************************************************************/

void IO_Write(ioAddress addr, ioData data)
{
    setExpectedAndActual(FLASH_WRITE, addr, data);

    failWhenNoUnusedExpectations(report_write_but_no_unused_expectations);
    failWhen(expectationKindDoesNotMatch(), report_expected_read_was_write);
    failWhen(expectationAddressDoesNotMatch(), report_write_does_not_match);
    failWhen(expectationDataDoesNotMatch(), report_write_does_not_match);

    getExpectationCount++;
}

ioData IO_Read(ioAddress addr)
{
    setExpectedAndActual(FLASH_READ, addr, NO_EXPECTED_VALUE);
    
    failWhenNoUnusedExpectations(report_read_but_no_unused_expectations);
    failWhen(expectationKindDoesNotMatch(), report_expected_write_was_read);
    failWhen(expectationAddressDoesNotMatch(), report_read_wrong_address);

    return expectations[getExpectationCount++].data;
}