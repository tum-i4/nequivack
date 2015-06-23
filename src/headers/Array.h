#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdbool.h>

#include "Function.h"
#include "Structure.h"

bool arrayContainsString(char**, unsigned, char*);
bool arrayContainsFunction(Function*, unsigned, Function);
bool arrayContainsStruct(Structure*, unsigned, Structure);

void appendStringToArray(const char*, char***, unsigned*);

#endif
