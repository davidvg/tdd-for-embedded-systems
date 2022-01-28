#ifndef _RANDOM_MINUTE_H
#define _RANDOM_MINUTE_H


void RandomMinute_Create(int bound);

/**
 * We use a function pointer to select the function that will return the
 * random minute. In production it will be the actual function, but in the tests
 * it will be a double.
 */
extern int (*RandomMinute_Get)(void);



#endif