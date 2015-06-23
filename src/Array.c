#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "headers/Array.h"
#include "headers/Function.h"
#include "headers/LibraryHelper.h"
#include "headers/String.h"
#include "headers/Structure.h"

bool arrayContainsString(char** strings, unsigned numStrings, char* string) {
	for (unsigned i = 0; i < numStrings; i++) {
		if (stringsEqual(strings[i], string)) {
			return true;
		}
	}

	return false;
}

bool arrayContainsFunction(Function* functions, unsigned numFunctions, Function function) {
	for (unsigned i = 0; i < numFunctions; i++) {
		if (functionsEqual(functions[i], function)) {
			return true;
		}
	}

	return false;
}

bool arrayContainsStruct(Structure* structures, unsigned numStructures, Structure structure) {
	for (unsigned i = 0; i < numStructures; i++) {
		if (structsEqual(structures[i], structure)) {
			return true;
		}
	}

	return false;
}

void appendStringToArray(const char* newString, char*** strings, unsigned* numStrings) {
	*strings = reallocateMemory(*strings, (*numStrings + 1) * sizeof(char*));
	(*strings)[*numStrings] = allocateMemory(BUFSIZ);
	strcpy((*strings)[*numStrings], newString);
	++(*numStrings);
}
