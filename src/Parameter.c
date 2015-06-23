#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "headers/Parameter.h"
#include "headers/String.h"

void enumerateParametersWithPrefix(Parameter* parameters, const unsigned numParameters, const char* prefix) {
	for (unsigned parameterIndex = 0; parameterIndex < numParameters; parameterIndex++) {
		char* parameterName = concatenateStrings(3, prefix, "_", getStringFromInt(parameterIndex));
		parameters[parameterIndex].parameterName = parameterName;
	}
}

bool parameterIsPointer(const char* parameterType) {
	return stringEndsWith(parameterType, " *");
}

bool parameterIsArray(const char* parameterType) {
	return stringEndsWith(parameterType, " []");
}

bool parameterIsStruct(const char* parameterType) {
	return stringStartsWith(parameterType, "struct ");
}

char* getArrayType(const char* parameterType) {
	if (!parameterIsArray(parameterType)) {
		puts("Parameter type is no array!");
		exit(1);
	}

	const char* arrayDeclaration = " []";

	return getSubstring(parameterType, 1, strlen(parameterType) - strlen(arrayDeclaration));
}
