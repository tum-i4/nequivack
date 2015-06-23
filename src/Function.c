#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "headers/Function.h"
#include "headers/LibraryHelper.h"
#include "headers/Parameter.h"
#include "headers/String.h"

char** getFunctionSignatures(Function* functions, unsigned numFunctions) {
	char** functionSignatures = allocateMemory(numFunctions * sizeof(char*));

	for (unsigned i = 0; i < numFunctions; i++) {
		functionSignatures[i] = getFunctionSignature(functions[i]);
	}

	return functionSignatures;
}

char* getFunctionSignature(Function function) {
	int signatureLength = calculateSignatureLengthForFunction(function);
	char* signature = allocateMemory(signatureLength + 1); // NUL

	buildSignatureForFunction(signature, function);

	return signature;
}

static int calculateSignatureLengthForFunction(Function function) {
	int signatureLength = 4; // SPACE();

	signatureLength += strlen(function.returnType) + strlen(function.name);

	for (int i = 0; i < function.numParameters; i++) {
		signatureLength += strlen(function.parameters[i].parameterType);

		if (i != function.numParameters - 1) {
			signatureLength += 2; // ,SPACE
		}
	}

	return signatureLength;
}

static void buildSignatureForFunction(char* signature, Function function) {
	strcpy(signature, function.returnType);
	strcat(signature, " ");
	strcat(signature, function.name);
	strcat(signature, "(");

	for (int i = 0; i < function.numParameters; i++) {
		strcat(signature, function.parameters[i].parameterType);

		if (i != function.numParameters - 1) {
			strcat(signature, ", ");
		}
	}

	strcat(signature, ");");
}

void sortFunctions(Function* functions, unsigned numFunctions) {
	qsort(functions, numFunctions, sizeof(Function), compareFunctionPointers);
}

bool allFunctionsEqual(Function* functions1, Function* functions2, const unsigned numFunctions) {
	for (unsigned i = 0; i < numFunctions; i++) {
		if (!functionsEqual(functions1[i], functions2[i])) {
			return false;
		}
	}

	return true;
}

bool functionsEqual(Function f1, Function f2) {
	return compareFunctions(f1, f2) == 0;
}

int compareFunctionPointers(const void* f1, const void* f2) {
	const Function function1 = *((Function*) f1);
	const Function function2 = *((Function*) f2);

	return compareFunctions(function1, function2);
}

int compareFunctions(const Function f1, const Function f2) {
	char* signatureF1 = getFunctionSignature(f1);
	char* signatureF2 = getFunctionSignature(f2);

	unsigned lengthOfSignature1 = strlen(signatureF1);
	unsigned lengthOfSignature2 = strlen(signatureF2);
	unsigned lengthOfShorterSignature = lengthOfSignature1;

	if (lengthOfSignature2 < lengthOfSignature1) {
		lengthOfShorterSignature = lengthOfSignature2;
	}

	for (unsigned i = 0; i < lengthOfShorterSignature; i++) {
		if (signatureF1[i] > signatureF2[i]) {
			return 1;
		} else if (signatureF1[i] < signatureF2[i]) {
			return -1;
		}
	}

	if (lengthOfSignature1 < lengthOfSignature2) {
		return 1;
	} else if (lengthOfSignature1 > lengthOfSignature2) {
		return -1;
	}

	return 0;
}

char** getFunctionNames(Function* functions, unsigned numFunctions) {
	char** functionNames = allocateMemory(numFunctions * sizeof(char*));

	for (unsigned i = 0; i < numFunctions; i++) {
		functionNames[i] = getFunctionName(functions[i]);
	}

	return functionNames;
}

char* getFunctionName(Function function) {
	int functionNameLength = strlen(function.name);
	char* functionName = allocateMemory(functionNameLength + 1);

	strcpy(functionName, function.name);

	return functionName;
}

void removePrefixFromFunctions(Function* functions, const unsigned numFunctions, const char* prefix) {
	for (unsigned i = 0; i < numFunctions; i++) {
		removePrefixFromString(&(functions[i].name), prefix);
	}
}

void removeFunctionsWithPrefix(Function** functions, unsigned* numFunctions, const char* prefix) {
	Function* functionsWithoutPrefix = allocateMemory(sizeof(Function));
	unsigned numFunctionsWithoutPrefix = 0;

	for (unsigned i = 0; i < *numFunctions; i++) {
		if (!functionIsPrefixed((*functions)[i], prefix)) {
			++numFunctionsWithoutPrefix;
			functionsWithoutPrefix = reallocateMemory(functionsWithoutPrefix, numFunctionsWithoutPrefix * sizeof(Function));
			functionsWithoutPrefix[numFunctionsWithoutPrefix - 1] = (*functions)[i];
		}
	}

	(*numFunctions) = numFunctionsWithoutPrefix;
	(*functions) = functionsWithoutPrefix;
}

static bool functionIsPrefixed(Function function, const char* prefix) {
	char* functionName = getFunctionName(function);
	return stringStartsWith(functionName, prefix);
}
