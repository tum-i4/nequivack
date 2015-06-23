#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <stdbool.h>

typedef struct Parameter {
	char* parameterType;
	char* parameterName;
	unsigned inArraySize;
	char* parameterValue;
} Parameter;

void enumerateParametersWithPrefix(Parameter*, const unsigned, const char*);

bool parameterIsPointer(const char*);
bool parameterIsArray(const char*);
bool parameterIsStruct(const char*);

char* getArrayType(const char*);

#endif
