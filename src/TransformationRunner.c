#include <stdio.h>
#include <stdlib.h>

#include "headers/String.h"

int main(int argc, char** argv) {
	if (argc != 5) {
		printf("program '%s' halted \n", argv[0]);
		printf("exactly four arguments (transformation command line, transformation prefix, input file, output file without ending) expected \n");

		return EXIT_FAILURE;
	}

	const char* transformationCommand = argv[1];
	const char* transformationPrefix = argv[2];
	const char* inputFile = argv[3];
	const char* outputFile = argv[4];

	printf("transformationCommand is '%s'\n", transformationCommand);
	printf("transformationPrefix is '%s'\n", transformationPrefix);
	printf("inputFile is '%s'\n", inputFile);
	printf("outputFile is '%s'\n", outputFile);

	char* executingTransformationCommand = replaceInString(transformationCommand, "%IN", inputFile);
	executingTransformationCommand = replaceInString(executingTransformationCommand, "%OUT", outputFile);
	executingTransformationCommand = replaceInString(executingTransformationCommand, "%PRE", transformationPrefix);

	printf("\nrun following transformation command:\n'%s'\n", executingTransformationCommand);

	int returnOfTransformation = system(executingTransformationCommand);

	if (returnOfTransformation) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
