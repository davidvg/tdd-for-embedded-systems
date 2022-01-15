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

static char * report_expected_read_was_write = 
    "Expected IO_Read(0x0) would return 0x1\n"
    "\t        But was IO_Write(0x0, 0x0)";

static char * report_expected_write_was_read = 
    "Expected IO_Write(0x0, 0x1)\n"
    "\t        But was IO_Read(0x0)";

/*******************************************************************************
 * Failing methods
 ******************************************************************************/

/**
 * @brief Checks that the Expectation's kind is NOT equal to the passed value
 * 
 * @param kind 
 * @return int 
 */
static int expectationKindIsNot(int kind)
{
    return expectations[getExpectationCount].kind != kind;
}

/**
 * @brief Checks if a failing condition is met and fails with a message
 * 
 * @param condition 
 * @param msg 
 */
static void failWhen(int condition, char * msg)
{
    if (condition)
    {
        FAIL_TEXT_C(msg);
    }
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
 * @brief Adds a new Expectation to the expectations array and increments
 * @brief the count in setExpectationCount
 * 
 * @param addr 
 * @param data 
 */
static void recordExpectation(Operation kind, ioAddress addr, ioData data)
{
    expectations[setExpectationCount].kind = kind;
    expectations[setExpectationCount].addr = addr;
    expectations[setExpectationCount].data = data;
    // printExpectation(setExpectationCount);
    setExpectationCount++;
}

/**
 * @brief Set the expected Expectation to the next recorded Expectation and
 * @brief the actual Expectation to the intercepted Expectation
 * 
 * @param addr 
 * @param data 
 */
static void setExpectedAndActual(ioAddress addr, ioData data)
{
    expected.addr = expectations[getExpectationCount].addr;
    expected.data = expectations[getExpectationCount].data;
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
    free(expectations);
    maxExpectationCount = 0;
    setExpectationCount = 0;
    getExpectationCount = 0;
}

void MockIO_Expect_Write(ioAddress addr, ioData data)
{
    recordExpectation(FLASH_WRITE, addr, data);
}

void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData data)
{
    recordExpectation(FLASH_READ, addr, data);
}

/*******************************************************************************
 * Interface methods
 ******************************************************************************/

void IO_Write(ioAddress addr, ioData data)
{
    setExpectedAndActual(addr, data);

    failWhen(expectationKindIsNot(FLASH_WRITE),
             report_expected_read_was_write);

    getExpectationCount++;
}

ioData IO_Read(ioAddress addr)
{
    setExpectedAndActual(addr, NO_EXPECTED_VALUE);
    
    failWhen(expectationKindIsNot(FLASH_READ),
             report_expected_write_was_read);

    return expectations[getExpectationCount++].data;
}