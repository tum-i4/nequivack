#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/Function.h"
#include "headers/FunctionExtractor.h"
#include "headers/String.h"

static bool headersContainSameFunctions(const char*, const char*, const char*, const char*);

int main(int argc, char** argv) {
	if (argc != 5) {
		printf("program '%s' halted \n", argv[0]);
		printf("exactly four arguments (original header, original prefix, transformed header, transformation prefix) expected \n");

		return EXIT_FAILURE;
	}

	const char* originalHeader = argv[1];
	const char* originalPrefix = argv[2];
	const char* transformedHeader = argv[3];
	const char* transformedPrefix = argv[4];

	printf("originalHeader is '%s'\n", originalHeader);
	printf("originalPrefix is '%s'\n", originalPrefix);
	printf("transformedHeader is '%s'\n", transformedHeader);
	printf("transformedPrefix is '%s'\n", transformedPrefix);

	puts("");

	if (headersContainSameFunctions(originalHeader, originalPrefix, transformedHeader, transformedPrefix)) {
		puts("The header files contain the same functions.");
		puts("Continue with checking non-equivalence.");

		return EXIT_SUCCESS;
	} else {
		puts("Header files are non-equivalent.");
		puts("Therefore non-equivalence is shown.");

		return EXIT_FAILURE;
	}
}

static bool headersContainSameFunctions(const char* originalHeader, const char* originalPrefix, const char* transformedHeader, const char* transformedPrefix) {
	unsigned numOriginalFunctions, numTransformedFunctions;
	Function* originalFunctions;
	Function* transformedFunctions;

	getFunctions(originalHeader, &originalFunctions, &numOriginalFunctions);
	getFunctions(transformedHeader, &transformedFunctions, &numTransformedFunctions);

	if (numOriginalFunctions != numTransformedFunctions) {
		return false;
	}

	sortFunctions(originalFunctions, numOriginalFunctions);
	sortFunctions(transformedFunctions, numTransformedFunctions);

	char** originalFunctionNames = getFunctionNames(originalFunctions, numOriginalFunctions);
	char** transformedFunctionNames = getFunctionNames(transformedFunctions, numTransformedFunctions);

	removePrefixFromStrings(originalFunctionNames, numOriginalFunctions, originalPrefix);
	removePrefixFromStrings(transformedFunctionNames, numTransformedFunctions, transformedPrefix);

	for (unsigned i = 0; i < numOriginalFunctions; i++) {
		originalFunctions[i].name = originalFunctionNames[i];
		transformedFunctions[i].name = transformedFunctionNames[i];
	}

	return allFunctionsEqual(originalFunctions, transformedFunctions, numOriginalFunctions);
}
