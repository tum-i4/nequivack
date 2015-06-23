#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/Array.h"
#include "headers/CodeGenerator.h"
#include "headers/LibraryHelper.h"
#include "headers/String.h"

void generateCodeForSearchPathInclude(const char* header, char*** codeLines, unsigned* numLines) {
	char* relativeInclude = concatenateStrings(3, "#include <", header, ">");
	addCodeLine(relativeInclude, codeLines, numLines);
	free(relativeInclude);
}

void generateCodeForRelativeInclude(const char* header, char*** codeLines, unsigned* numLines) {
	char* relativeInclude = concatenateStrings(3, "#include \"", header, "\"");
	addCodeLine(relativeInclude, codeLines, numLines);
	free(relativeInclude);
}

void addCodeLine(const char* newCodeLine, char*** codeLines, unsigned* numLines) {
	appendStringToArray(newCodeLine, codeLines, numLines);
}
