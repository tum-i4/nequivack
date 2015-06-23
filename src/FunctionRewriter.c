#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <clang-c/Index.h>

#include "headers/File.h"
#include "headers/Function.h"
#include "headers/FunctionExtractor.h"
#include "headers/LibraryHelper.h"
#include "headers/String.h"
#include "headers/Tokenizer.h"

void addPrefixToFunctionNamesInCodeLines(char**, const unsigned, const char*, char***, const unsigned);
void addPrefixToFunctionNameInCodeLines(char*, const char*, char***, const unsigned);
void addPrefixToFunctionNameInCodeLine(const char*, const char*, char**);
void getIndicesOfFunctionNameInCodeLine(const char*, const char*, int**, unsigned*);
void removeIncorrectIndicesForFunctionNameInCodeLine(const char*, const char*, int**, unsigned*);
void recalculateIndicesWithPrefix(const char*, int**, unsigned);
void insertPrefixAtIndicesInCodeLine(const char*, char**, const int*, const unsigned);

int main(int argc, char** argv) {
	if (argc < 4) {
		printf("program '%s' halted \n", argv[0]);
		printf("at least three arguments (input source file, output source file, function prefix) expected \n");
		printf("in addition a fourth argument (comma separated list of function names) could be given \n");

		return EXIT_FAILURE;
	}

	const char* sourceFile = argv[1];
	const char* outputFile = argv[2];
	const char* functionPrefix = argv[3];
	char* functionList = argv[4];

	printf("sourceFile is '%s'\n", sourceFile);
	printf("outputFile is '%s'\n", outputFile);
	printf("functionPrefix is '%s'\n", functionPrefix);

	unsigned numCodeLines, numFunctions;
	char** codeLines;
	char** functionNames;
	Function* functions;

	if (functionList) {
		printf("functionList is '%s'\n", functionList);
		splitStringOnDelimiter(functionList, ",", &functionNames, &numFunctions);
	} else {
		getFunctions(sourceFile, &functions, &numFunctions);
		functionNames = getFunctionNames(functions, numFunctions);
	}

	readFileToStringArray(sourceFile, &codeLines, &numCodeLines);
	addPrefixToFunctionNamesInCodeLines(functionNames, numFunctions, functionPrefix, &codeLines, numCodeLines);
	writeStringArrayToFile(outputFile, codeLines, numCodeLines);

	free(codeLines);
	free(functionNames);

	return EXIT_SUCCESS;
}

void addPrefixToFunctionNamesInCodeLines(char** functionNames, const unsigned numFunctions, const char* prefix, char*** codeLines, const unsigned numCodeLines) {
	for (unsigned functionIndex = 0; functionIndex < numFunctions; functionIndex++) {
		addPrefixToFunctionNameInCodeLines(functionNames[functionIndex], prefix, codeLines, numCodeLines);
	}
}

void addPrefixToFunctionNameInCodeLines(char* functionName, const char* prefix, char*** codeLines, const unsigned numCodeLines) {
	for (unsigned arrayEntry = 0; arrayEntry < numCodeLines; arrayEntry++) {
		addPrefixToFunctionNameInCodeLine(prefix, functionName, &(*codeLines)[arrayEntry]);
	}
}

void addPrefixToFunctionNameInCodeLine(const char* prefix, const char* functionName, char** codeLine) {
	unsigned numIndices;
	int* indices;

	getIndicesOfFunctionNameInCodeLine(functionName, *codeLine, &indices, &numIndices);
	removeIncorrectIndicesForFunctionNameInCodeLine(functionName, *codeLine, &indices, &numIndices);
	recalculateIndicesWithPrefix(prefix, &indices, numIndices);
	insertPrefixAtIndicesInCodeLine(prefix, codeLine, indices, numIndices);
}

void getIndicesOfFunctionNameInCodeLine(const char* functionName, const char* codeLine, int** indices, unsigned* numIndices) {
	*numIndices = 0;
	*indices = allocateMemory(sizeof(int));

	int indexOfFunctionName = 0;

	while (1) {
		char* occurence = strstr(codeLine + indexOfFunctionName, functionName);

		if (!occurence) {
			// Function name is not present in this code line.
			return;
		}

		indexOfFunctionName = occurence - codeLine;

		(*indices) = reallocateMemory((*indices), ((*numIndices) + 1) * sizeof(int));
		(*indices)[*numIndices] = indexOfFunctionName;
		++(*numIndices);

		// Next function must be after current.
		indexOfFunctionName += strlen(functionName);
	}
}

void removeIncorrectIndicesForFunctionNameInCodeLine(const char* functionName, const char* codeLine, int** indices, unsigned* numIndices) {
	int* correctIndices = allocateMemory(sizeof(int));
	unsigned numCorrectIndices = 0;

	// All of these chars signal that the declaration is bigger.
	const char* badChars = "_$AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789";

	for (unsigned i = 0; i < *numIndices; i++) {
		int index = (*indices)[i];
		int indexBeforeFunction = index - 1;
		int indexAfterFunction = index + strlen(functionName);

		if (indexBeforeFunction >= 0 && indexAfterFunction <= strlen(codeLine)) {
			if (!strchr(badChars, codeLine[indexBeforeFunction]) && !strchr(badChars, codeLine[indexAfterFunction])) {
				correctIndices = reallocateMemory(correctIndices, (numCorrectIndices + 1) * sizeof(int));
				correctIndices[numCorrectIndices] = index;
				++numCorrectIndices;
			}
		}
	}

	free(*indices);

	*numIndices = numCorrectIndices;
	*indices = correctIndices;
}

void recalculateIndicesWithPrefix(const char* prefix, int** indices, unsigned numIndices) {
	// Corrects the indices for each index before.
	unsigned prefixLength = strlen(prefix);

	for (unsigned i = 0; i < numIndices; i++) {
		(*indices)[i] = (*indices)[i] + (i * prefixLength);
	}
}

void insertPrefixAtIndicesInCodeLine(const char* prefix, char** codeLine, const int* indices, const unsigned numIndices) {
	for (unsigned i = 0; i < numIndices; i++) {
		insertSubstringAtPosition(codeLine, prefix, indices[i] + 1);
	}
}
