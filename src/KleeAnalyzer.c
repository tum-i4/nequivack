#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/File.h"
#include "headers/LibraryHelper.h"
#include "headers/String.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("program '%s' halted \n", argv[0]);
		printf("exactly two arguments (klee-last, output directory) expected \n");

		return EXIT_FAILURE;
	}

	const char* kleeLastDir = argv[1];
	const char* outputDir = argv[2];

	printf("kleeLastDir is '%s'\n", kleeLastDir);
	printf("outputDir is '%s'\n", outputDir);

	const char* errorFileName = "Errors.txt";
	const char* errorFileNamePattern = "test*.*.err";
	const char* assertionErrorEnding = ".assert.err";

	char* errorFile = allocateMemory(strlen(outputDir) + 1 + strlen(errorFileName) + 1);
	sprintf(errorFile, "%s/%s", outputDir, errorFileName);

	char* executingListCommand = allocateMemory(3 + strlen(kleeLastDir) + 1 + strlen(errorFileNamePattern) + 3 + strlen(outputDir) + 1 + strlen(errorFile) + 14 + 1);
	sprintf(executingListCommand, "ls %s/%s > %s 2> /dev/null", kleeLastDir, errorFileNamePattern, errorFile);

	if (system(executingListCommand)) {
		puts("\nResult: Programs seem to be equivalent.");
		puts("KLEE did not find any counterexamples or errors.");

		removeFile(errorFile);

		return EXIT_SUCCESS;
	}

	unsigned numErrors, numAssertionErrors = 0;
	char** errors;

	readFileToStringArray(errorFile, &errors, &numErrors);

	for (unsigned i = 0; i < numErrors; i++) {
		if (stringEndsWith(errors[i], assertionErrorEnding)) {
			printf("inc: %s, %s\n", errors[i], assertionErrorEnding);
			++numAssertionErrors;
		}
	}

	if (numAssertionErrors > 0) {
		puts("\nResult: Programs are not equivalent.");
		puts("KLEE found at least one counterexample.");
	} else if (numErrors) {
		puts("\nResult: Programs are probably not equivalent.");
		puts("KLEE found at least one error that has to be checked.");
	}

	removeFile(errorFile);

	return EXIT_FAILURE;
}
