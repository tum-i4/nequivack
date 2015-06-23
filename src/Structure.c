#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "headers/CodeGenerator.h"
#include "headers/LibraryHelper.h"
#include "headers/String.h"
#include "headers/Structure.h"

bool structsEqual(Structure s1, Structure s2) {
	return stringsEqual(s1.name, s2.name);
}

void generateCodeForStructs(Structure* structures, unsigned numStructs, char*** codeLines, unsigned* numLines) {
	for (unsigned i = 0; i < numStructs; i++) {
		generateCodeForStruct(structures[i], codeLines, numLines);
	}
}

void generateCodeForStruct(Structure structure, char*** codeLine, unsigned* numLines) {
	char* structName = concatenateStrings(2, structure.name, " {");
	addCodeLine(structName, codeLine, numLines);
	free(structName);

	for (unsigned i = 0; i < structure.numMembers; i++) {
		char* structMember = concatenateStrings(5, "\t", structure.members[i].parameterType, " ", structure.members[i].parameterName, ";");
		addCodeLine(structMember, codeLine, numLines);
		free(structMember);
	}

	addCodeLine("};\0", codeLine, numLines);
}

Structure* getStructWithType(char* type, Structure* structs, const unsigned numStructs) {
	for (unsigned i = 0; i < numStructs; i++) {
		if (stringStartsWith(type, structs[i].name)) {
			return structs + i;
		}
	}

	return NULL;
}

char* getBaseNameOfStruct(Structure structure) {
	char* structName = strdup(structure.name);
	removePrefixFromString(&structName, "struct ");
	return structName;
}
