#include "FormatOutput.h"


// Production: the function pointer points to printf()
int (*FormatOutput)(const char * format, ...) = printf;
