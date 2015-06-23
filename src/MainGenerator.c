#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/CodeGenerator.h"
#include "headers/File.h"
#include "headers/Function.h"
#include "headers/FunctionExtractor.h"
#include "headers/LibraryHelper.h"
#include "headers/Parameter.h"
#include "headers/String.h"
#include "headers/StructExtractor.h"

#define equivalenceCheckFunctionPrefix "checkEquivalenceOf"
#define equivalenceCheckMaxSize 1
#define symbolizationFunctionPrefix "createSymbolic"
#define symbolizationStructName "symbolicStructure"
#define testFunctionPrefix "test_"
#define returnParameterName "returnParameter"

void generateCodeForIncludes(const char*, const char*, char***, unsigned*);
void generateCodeForStructsSymbolization(Structure*, const unsigned, const char*, const char*, char***, unsigned*);
void generateCodeForStructSymbolization(Structure, Structure*, const unsigned, const char*, const char*, char***, unsigned*);
void generateCodeForStructsEquivalence(Structure*, const unsigned, char***, unsigned*);
void generateCodeForStructEquivalence(Structure, char***, unsigned*);
void generateCodeForFunctions(Structure*, const unsigned, Function*, const unsigned, const char*, const char*, char***, unsigned*);
static void generateCodeForFunction(Structure*, const unsigned, Function, const char*, const char*, char***, unsigned*);
static void generateCodeForParameters(Structure*, const unsigned, Parameter*, const unsigned, const char*, const char*, char**, char**, char***, unsigned*);
static void generateCodeForParameter(Structure*, const unsigned, Parameter, const char*, const char*, char**, char**, char***, unsigned*);
static void generateCodeForPrimitiveParameter(Parameter, char**, char**, char***, unsigned*);
static void generateCodeForArrayParameter(Parameter, const char*, const char*, char**, char**, char***, unsigned*);
static void generateCodeForArrayCopy(const char*, unsigned, const char*, const char*, char***, unsigned*);
static void generateCodeForStructParameter(Structure*, const unsigned, Parameter, const char*, const char*, char**, char**, char***, unsigned*);
static void generateCodeForStructCopy(const char*, const char*, const char*, const char*, char***, unsigned*);
static void appendCallParameter(char**, const char*);
static void generateCodeForCheckingEquivalence(Function, const char*, const char*, const char*, const char*, char***, unsigned*);
static void generateCodeForAssertion(const char*, const char*, const char*, const unsigned, char***, unsigned*);
static void generateCodeForReturnParameters(Function, const char*, const char*, const char*, const char*, const char*, const char*, char***, unsigned*);
static void generateCodeForPrimitiveAssertion(const char*, const char*, char***, unsigned*);
static void generateCodeForArrayAssertion(unsigned, const char*, const char*, char***, unsigned*);
static void generateCodeForStructAssertion(const char*, const char*, const char*, char***, unsigned*);
void generateCodeForMain(Function* functions, const unsigned, char***, unsigned*);
static void generateCodeForFunctionCalls(Function*, unsigned, char***, unsigned*);
static void generateCodeForFunctionCall(Function, char***, unsigned*);

int main(int argc, char** argv) {
	if (argc != 9) {
		printf("program '%s' halted \n", argv[0]);
		printf("exactly five arguments (original source file, annotation, structs header, original header file, transformed header file, original prefix, transformed prefix and KLEE main file) expected \n");

		return EXIT_FAILURE;
	}

	const char* originalSourceFile = argv[1];
	const char* annotation = argv[2];
	const char* structsHeader = argv[3];
	const char* originalHeader = argv[4];
	const char* transformedHeader = argv[5];
	const char* originalPrefix = argv[6];
	const char* transformedPrefix = argv[7];
	const char* kleeMainFile = argv[8];

	printf("originalSourceFile is '%s'\n", originalSourceFile);
	printf("annotation is '%s'\n", annotation);
	printf("structsHeader is '%s'\n", structsHeader);
	printf("originalHeader is '%s'\n", originalHeader);
	printf("transformedHeader is '%s'\n", transformedHeader);
	printf("originalPrefix is '%s'\n", originalPrefix);
	printf("transformedPrefix is '%s'\n", transformedPrefix);
	printf("kleeMainFile is '%s'\n", kleeMainFile);

	unsigned numFunctions, numStructs, numCodeLines = 0;
	Function* functions;
	Structure* structs;
	char** mainFileCodeLines = allocateMemory(sizeof(char*));

	getStructs(structsHeader, &structs, &numStructs);
	getAnnotatedFunctions(originalSourceFile, annotation, &functions, &numFunctions);
	removePrefixFromFunctions(functions, numFunctions, originalPrefix);

	generateCodeForIncludes(originalHeader, transformedHeader, &mainFileCodeLines, &numCodeLines);
	generateCodeForStructsSymbolization(structs, numStructs, originalPrefix, transformedPrefix, &mainFileCodeLines, &numCodeLines);
	generateCodeForStructsEquivalence(structs, numStructs, &mainFileCodeLines, &numCodeLines);
	generateCodeForFunctions(structs, numStructs, functions, numFunctions, originalPrefix, transformedPrefix, &mainFileCodeLines, &numCodeLines);
	generateCodeForMain(functions, numFunctions, &mainFileCodeLines, &numCodeLines);

	writeStringArrayToFileWithSuffix(kleeMainFile, mainFileCodeLines, numCodeLines, "\n");

	free(structs);
	free(functions);
	free(mainFileCodeLines);

	return EXIT_SUCCESS;
}

void generateCodeForIncludes(const char* originalHeaderFile, const char* transformedHeaderFile, char*** codeLines, unsigned* numLines) {
	const char* kleeHeader = "klee/klee.h";
	const char* originalHeader = getFileNameFromPath(originalHeaderFile);
	const char* transformedHeader = getFileNameFromPath(transformedHeaderFile);

	generateCodeForSearchPathInclude("assert.h", codeLines, numLines);
	generateCodeForSearchPathInclude("stdlib.h", codeLines, numLines);
	generateCodeForSearchPathInclude("string.h", codeLines, numLines);

	generateCodeForRelativeInclude(kleeHeader, codeLines, numLines);
	generateCodeForRelativeInclude(originalHeader, codeLines, numLines);
	generateCodeForRelativeInclude(transformedHeader, codeLines, numLines);
}

void generateCodeForStructsSymbolization(Structure* structs, const unsigned numStructs, const char* originalPrefix, const char* transformedPrefix, char*** codeLines, unsigned* numLines) {
	for (unsigned i = 0; i < numStructs; i++) {
		generateCodeForStructSymbolization(structs[i], structs, numStructs, originalPrefix, transformedPrefix, codeLines, numLines);
	}
}

void generateCodeForStructSymbolization(Structure structure, Structure* structs, const unsigned numStructs, const char* originalPrefix, const char* transformedPrefix, char*** codeLines, unsigned* numLines) {
	char* methodDeclaration = concatenateStrings(6, "\n", structure.name, "* ", symbolizationFunctionPrefix, getBaseNameOfStruct(structure), "() {");
	char* structDeclaration = concatenateStrings(7, "\t", structure.name, "* ", symbolizationStructName, " = malloc(sizeof(", structure.name, "));\n");
	char* returnStruct = concatenateStrings(3, "\n\treturn ", symbolizationStructName, ";");

	addCodeLine(methodDeclaration, codeLines, numLines);
	generateCodeForParameters(structs, numStructs, structure.members, structure.numMembers, originalPrefix, transformedPrefix, NULL, NULL, codeLines, numLines);

	addCodeLine(structDeclaration, codeLines, numLines);
	for (unsigned i = 0; i < structure.numMembers; i++) {
		char* memberAssignment = concatenateStrings(7, "\t", symbolizationStructName, "->", structure.members[i].parameterName, " = ", structure.members[i].parameterName, ";");
		addCodeLine(memberAssignment, codeLines, numLines);
		free(memberAssignment);
	}

	addCodeLine(returnStruct, codeLines, numLines);
	addCodeLine("}\0", codeLines, numLines);

	free(methodDeclaration);
	free(structDeclaration);
	free(returnStruct);
}

void generateCodeForStructsEquivalence(Structure* structs, const unsigned numStructs, char*** codeLines, unsigned* numLines) {
	for (unsigned i = 0; i < numStructs; i++) {
		generateCodeForStructEquivalence(structs[i], codeLines, numLines);
	}
}

void generateCodeForStructEquivalence(Structure structure, char*** codeLines, unsigned* numLines) {
	const char* lhsInput = "lhs";
	const char* rhsInput = "rhs";
	char* methodDeclaration = concatenateStrings(12, "\nvoid ", equivalenceCheckFunctionPrefix, getBaseNameOfStruct(structure), "(", structure.name, "* ", lhsInput, ", ", structure.name, "* ", rhsInput, ") {");

	addCodeLine(methodDeclaration, codeLines, numLines);

	for (unsigned i = 0; i < structure.numMembers; i++) {
		char* type = structure.members[i].parameterType;
		char* lhsMemberName = concatenateStrings(3, lhsInput, "->", structure.members[i].parameterName);
		char* rhsMemberName = concatenateStrings(3, rhsInput, "->", structure.members[i].parameterName);

		generateCodeForAssertion(type, lhsMemberName, rhsMemberName, equivalenceCheckMaxSize, codeLines, numLines);

		free(type);
		free(lhsMemberName);
		free(rhsMemberName);
	}

	addCodeLine("}\0", codeLines, numLines);

	free(methodDeclaration);
}

void generateCodeForFunctions(Structure* structs, const unsigned numStructs, Function* functions, const unsigned numFunctions, const char* originalPrefix, const char* transformedPrefix, char*** codeLines,
		unsigned* numLines) {
	for (unsigned i = 0; i < numFunctions; i++) {
		generateCodeForFunction(structs, numStructs, functions[i], originalPrefix, transformedPrefix, codeLines, numLines);
	}
}

static void generateCodeForFunction(Structure* structs, const unsigned numStructs, Function function, const char* originalPrefix, const char* transformedPrefix, char*** codeLines, unsigned* numLines) {
	enumerateParametersWithPrefix(function.parameters, function.numParameters, function.name);

	char* methodDeclaration = concatenateStrings(4, "\nvoid ", testFunctionPrefix, function.name, "() {");
	char* originalCallParameters = "";
	char* transformedCallParameters = "";

	addCodeLine(methodDeclaration, codeLines, numLines);
	generateCodeForParameters(structs, numStructs, function.parameters, function.numParameters, originalPrefix, transformedPrefix, &originalCallParameters, &transformedCallParameters, codeLines, numLines);
	generateCodeForCheckingEquivalence(function, originalPrefix, transformedPrefix, originalCallParameters, transformedCallParameters, codeLines, numLines);
	addCodeLine("}\0", codeLines, numLines);

	free(methodDeclaration);
}

static void generateCodeForParameters(Structure* structs, const unsigned numStructs, Parameter* parameters, const unsigned numParameters, const char* originalPrefix, const char* transformedPrefix,
		char** originalCallParameters, char** transformedCallParameters, char*** codeLines, unsigned* numLines) {
	for (unsigned parameterIndex = 0; parameterIndex < numParameters; parameterIndex++) {
		generateCodeForParameter(structs, numStructs, parameters[parameterIndex], originalPrefix, transformedPrefix, originalCallParameters, transformedCallParameters, codeLines, numLines);
	}
}

static void generateCodeForParameter(Structure* structs, const unsigned numStructs, Parameter parameter, const char* originalPrefix, const char* transformedPrefix, char** originalCallParameters,
		char** transformedCallParameters, char*** codeLines, unsigned* numLines) {
	if (parameterIsArray(parameter.parameterType)) {
		generateCodeForArrayParameter(parameter, originalPrefix, transformedPrefix, originalCallParameters, transformedCallParameters, codeLines, numLines);
	} else if (parameterIsStruct(parameter.parameterType)) {
		generateCodeForStructParameter(structs, numStructs, parameter, originalPrefix, transformedPrefix, originalCallParameters, transformedCallParameters, codeLines, numLines);
	} else {
		generateCodeForPrimitiveParameter(parameter, originalCallParameters, transformedCallParameters, codeLines, numLines);
	}
}

static void generateCodeForPrimitiveParameter(Parameter parameter, char** originalCallParameters, char** transformedCallParameters, char*** codeLines, unsigned* numLines) {
	char* parameterValue = "";

	if (parameter.parameterValue) {
		parameterValue = concatenateStrings(2, " = ", parameter.parameterValue);
	}

	char* declaration = concatenateStrings(6, "\t", parameter.parameterType, " ", parameter.parameterName, parameterValue, ";");
	addCodeLine(declaration, codeLines, numLines);
	free(declaration);

	if (parameter.parameterValue) {
		addCodeLine("\0", codeLines, numLines);
	}

	if (!parameter.parameterValue) {
		char* symbolization = concatenateStrings(7, "\tklee_make_symbolic(&", parameter.parameterName, ", sizeof(", parameter.parameterName, "), \"", parameter.parameterName, "\");\n");
		addCodeLine(symbolization, codeLines, numLines);
		free(symbolization);
	}

	appendCallParameter(originalCallParameters, parameter.parameterName);
	appendCallParameter(transformedCallParameters, parameter.parameterName);
}

static void generateCodeForArrayParameter(Parameter parameter, const char* originalPrefix, const char* transformedPrefix, char** originalCallParameters, char** transformedCallParameters, char*** codeLines,
		unsigned* numLines) {
	const char* type = getArrayType(parameter.parameterType);
	char* parameterValue = "";

	if (parameter.parameterValue) {
		parameterValue = concatenateStrings(2, " = ", parameter.parameterValue);
	}

	char* declaration = concatenateStrings(9, "\t", type, " ", parameter.parameterName, "[", getStringFromInt(parameter.inArraySize), "]", parameterValue, ";");
	char* originalCopyName = concatenateStrings(2, originalPrefix, parameter.parameterName);
	char* transformedCopyName = concatenateStrings(2, transformedPrefix, parameter.parameterName);

	addCodeLine(declaration, codeLines, numLines);

	if (parameter.parameterValue) {
		addCodeLine("\0", codeLines, numLines);
	}

	if (!parameter.parameterValue) {
		char* symbolization = concatenateStrings(7, "\tklee_make_symbolic(", parameter.parameterName, ", sizeof(", parameter.parameterName, "), \"", parameter.parameterName, "\");\n");
		addCodeLine(symbolization, codeLines, numLines);
		free(symbolization);
	}

	generateCodeForArrayCopy(type, parameter.inArraySize, parameter.parameterName, originalCopyName, codeLines, numLines);
	generateCodeForArrayCopy(type, parameter.inArraySize, parameter.parameterName, transformedCopyName, codeLines, numLines);

	appendCallParameter(originalCallParameters, originalCopyName);
	appendCallParameter(transformedCallParameters, transformedCopyName);

	free(declaration);
	free(originalCopyName);
	free(transformedCopyName);
}

static void generateCodeForArrayCopy(const char* type, unsigned size, const char* originalName, const char* copyName, char*** codeLines, unsigned* numLines) {
	char* declaration = concatenateStrings(9, "\t", type, "* ", copyName, " = malloc(", getStringFromInt(size), " * sizeof(", type, "));");
	char* copy = concatenateStrings(9, "\tmemcpy(", copyName, ", ", originalName, ", ", getStringFromInt(size), " * sizeof(", type, "));\n");

	addCodeLine(declaration, codeLines, numLines);
	addCodeLine(copy, codeLines, numLines);

	free(declaration);
	free(copy);
}

static void generateCodeForStructParameter(Structure* structs, const unsigned numStructs, Parameter parameter, const char* originalPrefix, const char* transformedPrefix, char** originalCallParameters,
		char** transformedCallParameters, char*** codeLines, unsigned* numLines) {
	Structure* structure = getStructWithType(parameter.parameterType, structs, numStructs);

	const char* originalCopyStructName = concatenateStrings(2, originalPrefix, parameter.parameterName);
	const char* transformedCopyStructName = concatenateStrings(2, transformedPrefix, parameter.parameterName);

	char* dereference = parameterIsPointer(parameter.parameterType) ? "" : "*";
	char* parameterValue = "";

	if (parameter.parameterValue) {
		parameterValue = parameter.parameterValue;
	} else {
		parameterValue = concatenateStrings(4, dereference, symbolizationFunctionPrefix, getBaseNameOfStruct(*structure), "()");
	}

	char* declaration = concatenateStrings(7, "\t", parameter.parameterType, " ", parameter.parameterName, " = ", parameterValue, ";\n");
	addCodeLine(declaration, codeLines, numLines);
	free(declaration);

	generateCodeForStructCopy(structure->name, parameter.parameterType, parameter.parameterName, originalCopyStructName, codeLines, numLines);
	generateCodeForStructCopy(structure->name, parameter.parameterType, parameter.parameterName, transformedCopyStructName, codeLines, numLines);

	const char* originalCallParameter = concatenateStrings(2, dereference, originalCopyStructName);
	const char* transformedCallParameter = concatenateStrings(2, dereference, transformedCopyStructName);

	appendCallParameter(originalCallParameters, originalCallParameter);
	appendCallParameter(transformedCallParameters, transformedCallParameter);
}

static void generateCodeForStructCopy(const char* structType, const char* parameterType, const char* originalName, const char* copyName, char*** codeLines, unsigned* numLines) {
	const char* address = parameterIsPointer(parameterType) ? "" : "&";

	char* declaration = concatenateStrings(7, "\t", structType, " * ", copyName, " = malloc(sizeof(", structType, "));");
	char* copy = concatenateStrings(8, "\tmemcpy(", copyName, ", ", address, originalName, ", sizeof(", structType, "));\n");

	addCodeLine(declaration, codeLines, numLines);
	addCodeLine(copy, codeLines, numLines);

	free(declaration);
	free(copy);
}

static void appendCallParameter(char** callParameters, const char* newParameter) {
	if (!callParameters) {
		// Call parameters ignored.
		return;
	}

	if (strlen(*callParameters) == 0) {
		*callParameters = concatenateStrings(2, "", newParameter);
	} else {
		*callParameters = concatenateStrings(3, *callParameters, ", ", newParameter);
	}
}

static void generateCodeForCheckingEquivalence(Function function, const char* originalPrefix, const char* transformedPrefix, const char* originalCallParameters, const char* transformedCallParameters,
		char*** codeLines, unsigned* numLines) {
	char* originalFunctionReturnParameterName = concatenateStrings(2, originalPrefix, returnParameterName);
	char* transformedFunctionReturnParameterName = concatenateStrings(2, transformedPrefix, returnParameterName);

	generateCodeForReturnParameters(function, originalPrefix, transformedPrefix, originalCallParameters, transformedCallParameters, originalFunctionReturnParameterName, transformedFunctionReturnParameterName,
			codeLines, numLines);
	generateCodeForAssertion(function.returnType, originalFunctionReturnParameterName, transformedFunctionReturnParameterName, function.outArraySize, codeLines, numLines);
}

static void generateCodeForReturnParameters(Function function, const char* originalPrefix, const char* transformedPrefix, const char* originalCallParameters, const char* transformedCallParameters,
		const char* originalFunctionReturnParameterName, const char* transformedFunctionReturnParameterName, char*** codeLines, unsigned* numLines) {
	char* originalFunctionCall = concatenateStrings(5, originalPrefix, function.name, "(", originalCallParameters, ")");
	char* transformedFunctionCall = concatenateStrings(5, transformedPrefix, function.name, "(", transformedCallParameters, ")");
	char* originalFunctionReturnAssignment = concatenateStrings(7, "\t", function.returnType, " ", originalFunctionReturnParameterName, " = ", originalFunctionCall, ";");
	char* transformedFunctionReturnAssignment = concatenateStrings(7, "\t", function.returnType, " ", transformedFunctionReturnParameterName, " = ", transformedFunctionCall, ";");

	addCodeLine(originalFunctionReturnAssignment, codeLines, numLines);
	addCodeLine(transformedFunctionReturnAssignment, codeLines, numLines);

	free(originalFunctionCall);
	free(transformedFunctionCall);
	free(originalFunctionReturnAssignment);
	free(transformedFunctionReturnAssignment);
}

static void generateCodeForAssertion(const char* parameterType, const char* originalParameterName, const char* transformedParameterName, const unsigned maxArraySize, char*** codeLines, unsigned* numLines) {
	if (parameterIsStruct(parameterType)) {
		generateCodeForStructAssertion(parameterType, originalParameterName, transformedParameterName, codeLines, numLines);
	} else if (parameterIsPointer(parameterType)) {
		generateCodeForArrayAssertion(maxArraySize, originalParameterName, transformedParameterName, codeLines, numLines);
	} else {
		generateCodeForPrimitiveAssertion(originalParameterName, transformedParameterName, codeLines, numLines);
	}
}

static void generateCodeForPrimitiveAssertion(const char* originalFunctionReturnParameter, const char* transformedFunctionReturnParameter, char*** codeLines, unsigned* numLines) {
	char* assertion = concatenateStrings(5, "\n\tklee_assert(", originalFunctionReturnParameter, " == ", transformedFunctionReturnParameter, ");");
	addCodeLine(assertion, codeLines, numLines);
	free(assertion);
}

static void generateCodeForArrayAssertion(unsigned arraySize, const char* originalFunctionReturnParameter, const char* transformedFunctionReturnParameter, char*** codeLines, unsigned* numLines) {
	char* forLoop = concatenateStrings(3, "\n\tfor (unsigned i = 0; i < ", getStringFromInt(arraySize), "; i++) {");
	char* assertion = concatenateStrings(5, "\t\tklee_assert(", originalFunctionReturnParameter, "[i] == ", transformedFunctionReturnParameter, "[i]);");

	addCodeLine(forLoop, codeLines, numLines);
	addCodeLine(assertion, codeLines, numLines);
	addCodeLine("\t}\0", codeLines, numLines);

	free(forLoop);
	free(assertion);
}

static void generateCodeForStructAssertion(const char* structType, const char* originalFunctionReturnParameter, const char* transformedFunctionReturnParameter, char*** codeLines, unsigned* numLines) {
	const char* address = parameterIsPointer(structType) ? "" : "&";
	char* structName = strdup(structType);
	removePrefixFromString(&structName, "struct ");
	removeSuffixFromString(&structName, " *");

	char* functionCall = concatenateStrings(10, "\n\t", equivalenceCheckFunctionPrefix, structName, "(", address, originalFunctionReturnParameter, ", ", address, transformedFunctionReturnParameter, ");");
	addCodeLine(functionCall, codeLines, numLines);
	free(functionCall);
}

void generateCodeForMain(Function* functions, const unsigned numFunctions, char*** codeLines, unsigned* numLines) {
	addCodeLine("\nint main(int argc, char* argv[]) {", codeLines, numLines);
	generateCodeForFunctionCalls(functions, numFunctions, codeLines, numLines);
	addCodeLine("\n\treturn 0;", codeLines, numLines);
	addCodeLine("}\0", codeLines, numLines);
}

static void generateCodeForFunctionCalls(Function* functions, unsigned numFunctions, char*** codeLines, unsigned* numLines) {
	for (unsigned i = 0; i < numFunctions; i++) {
		generateCodeForFunctionCall(functions[i], codeLines, numLines);
	}
}

static void generateCodeForFunctionCall(Function function, char*** codeLines, unsigned* numLines) {
	char* functionCall = concatenateStrings(4, "\t", testFunctionPrefix, function.name, "();");
	addCodeLine(functionCall, codeLines, numLines);
	free(functionCall);
}
