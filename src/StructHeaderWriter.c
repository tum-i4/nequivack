#include <stdio.h>
#include <stdlib.h>

#include "headers/CodeGenerator.h"
#include "headers/File.h"
#include "headers/LibraryHelper.h"
#include "headers/String.h"
#include "headers/Structure.h"
#include "headers/StructExtractor.h"

#define headerName "STRUCTS_H"

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("program '%s' halted \n", argv[0]);
		printf("exactly three arguments (input source file and output header file) expected \n");

		return EXIT_FAILURE;
	}

	const char* sourceFile = argv[1];
	const char* outputFile = argv[2];

	printf("sourceFile is '%s'\n", sourceFile);
	printf("outputFile is '%s'\n", outputFile);

	unsigned numStructs, numLines = 0;
	Structure* structs;
	char** codeLines = allocateMemory(sizeof(char*));

	const char* headerIfNdef = concatenateStrings(2, "#ifndef ", headerName);
	const char* headerDefine = concatenateStrings(2, "#define ", headerName);
	const char* headerEndIf = "#endif";

	addCodeLine(headerIfNdef, &codeLines, &numLines);
	addCodeLine(headerDefine, &codeLines, &numLines);

	getStructs(sourceFile, &structs, &numStructs);
	generateCodeForStructs(structs, numStructs, &codeLines, &numLines);

	addCodeLine(headerEndIf, &codeLines, &numLines);

	writeStringArrayToFileWithSuffix(outputFile, codeLines, numLines, "\n");

	return EXIT_SUCCESS;
}
