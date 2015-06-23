#include <stdio.h>
#include <stdlib.h>

#include "headers/CodeGenerator.h"
#include "headers/File.h"
#include "headers/Function.h"
#include "headers/FunctionExtractor.h"
#include "headers/LibraryHelper.h"

int main(int argc, char** argv) {
	if (argc != 5) {
		printf("program '%s' halted \n", argv[0]);
		printf("exactly three arguments (input source file, output header file, annotation and struct header) expected \n");

		return EXIT_FAILURE;
	}

	const char* sourceFile = argv[1];
	const char* outputFile = argv[2];
	const char* annotation = argv[3];
	const char* structHeader = argv[4];

	printf("sourceFile is '%s'\n", sourceFile);
	printf("outputFile is '%s'\n", outputFile);
	printf("annotation is '%s'\n", annotation);
	printf("structHeader is '%s'\n", structHeader);

	unsigned numFunctions, numLines = 0;
	char** codeLines = allocateMemory(sizeof(char*));
	Function* functions;

	getAnnotatedFunctions(sourceFile, annotation, &functions, &numFunctions);
	sortFunctions(functions, numFunctions);

	char** functionSignatures = getFunctionSignatures(functions, numFunctions);

	generateCodeForRelativeInclude(structHeader, &codeLines, &numLines);

	for (unsigned i = 0; i < numFunctions; i++) {
		addCodeLine(functionSignatures[i], &codeLines, &numLines);
	}

	writeStringArrayToFileWithSuffix(outputFile, codeLines, numLines, "\n");

	return EXIT_SUCCESS;
}
