#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <stdbool.h>
#include "Parameter.h"

typedef struct Function {
	char* returnType;
	char* name;
	Parameter* parameters;
	unsigned numParameters;
	unsigned outArraySize;
} Function;

char** getFunctionSignatures(Function*, unsigned);
char* getFunctionSignature(Function);

static int calculateSignatureLengthForFunction(Function);
static void buildSignatureForFunction(char*, Function);

void sortFunctions(Function*, unsigned);

bool allFunctionsEqual(Function*, Function*, const unsigned);
bool functionsEqual(Function, Function);

int compareFunctionPointers(const void*, const void*);
int compareFunctions(const Function, const Function);

char** getFunctionNames(Function*, unsigned);
char* getFunctionName(Function);

void removePrefixFromFunctions(Function*, const unsigned, const char*);
void removeFunctionsWithPrefix(Function**, unsigned*, const char*);
static bool functionIsPrefixed(Function, const char*);

#endif
