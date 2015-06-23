#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <clang-c/Index.h>

#include "headers/Array.h"
#include "headers/ClangHelper.h"
#include "headers/Function.h"
#include "headers/FunctionExtractor.h"
#include "headers/LibraryHelper.h"
#include "headers/String.h"
#include "headers/Tokenizer.h"

#define DEFAULT_ARRAY_SIZE 1
#define ANNOTATION_IN_ARRAY_SIZE "inArraySize"
#define ANNOTATION_OUT_ARRAY_SIZE "outArraySize"
#define ANNOTATION_SYMBOLIC "sym"

void getFunctions(const char* sourceFile, Function** functionTarget, unsigned* numFunctions) {
	unsigned numTokens;
	CXToken* tokens;
	getTokensFromSourceFile(sourceFile, &tokens, &numTokens);

	getFunctionsFromTokens(sourceFile, functionTarget, numFunctions, tokens, numTokens);
}

void getAnnotatedFunctions(const char* sourceFile, const char* annotation, Function** functionTarget, unsigned* numFunctions) {
	if (!annotation) {
		return;
	}

	unsigned numTokens;
	CXToken* tokens;
	getTokensFromSourceFile(sourceFile, &tokens, &numTokens);

	getAnnotatedFunctionsFromTokens(sourceFile, annotation, functionTarget, numFunctions, tokens, numTokens);
}

void getFunctionsFromTokens(const char* sourceFile, Function** functionTarget, unsigned* numFunctions, CXToken* tokens, unsigned numTokens) {
	CXTranslationUnit translationUnit = getTranslationUnitFromSourceFile(sourceFile);

	*numFunctions = 0;
	*functionTarget = allocateMemory(sizeof(Function));

	CXCursor* cursors = allocateMemory(numTokens * sizeof(CXCursor));
	clang_annotateTokens(translationUnit, tokens, numTokens, cursors);

	for (unsigned index = 0; index < numTokens; index++) {
		CXCursor cursor = cursors[index];
		enum CXCursorKind tokenCursorKind = clang_getCursorKind(cursor);

		if (tokenCursorKind == CXCursor_FunctionDecl) {
			addNewFunctionWithCursor(cursor, functionTarget, numFunctions);
		}
	}

	clang_disposeTranslationUnit(translationUnit);
}

void getAnnotatedFunctionsFromTokens(const char* sourceFile, const char* annotation, Function** functionTarget, unsigned* numFunctions, CXToken* tokens, unsigned numTokens) {
	CXTranslationUnit translationUnit = getTranslationUnitFromSourceFile(sourceFile);

	*numFunctions = 0;
	*functionTarget = allocateMemory(sizeof(Function));

	CXCursor* cursors = allocateMemory(numTokens * sizeof(CXCursor));
	clang_annotateTokens(translationUnit, tokens, numTokens, cursors);

	for (unsigned index = 0; index < numTokens; index++) {
		CXCursor cursor = cursors[index];
		enum CXCursorKind tokenCursorKind = clang_getCursorKind(cursor);

		if (tokenCursorKind == CXCursor_FunctionDecl) {
			char* functionAnnotation = getAnnotationIfCorrect(translationUnit, annotation, tokens, index);

			if (functionAnnotation) {
				addNewFunctionWithCursorAndAnnotation(cursor, functionAnnotation, functionTarget, numFunctions);
			}
		}
	}

	clang_disposeTranslationUnit(translationUnit);
}

static char* getAnnotationIfCorrect(CXTranslationUnit translationUnit, const char* annotation, CXToken* tokens, unsigned tokenIndex) {
	if (tokenIndex > 0) { // First token cannot be an annotated function
		CXToken priorToken = tokens[tokenIndex - 1];
		CXTokenKind priorTokenKind = clang_getTokenKind(priorToken);

		if (priorTokenKind == CXToken_Comment) {
			const char* priorTokenSpelling = clang_getCString(clang_getTokenSpelling(translationUnit, priorToken));

			if (stringStartsWith(priorTokenSpelling, annotation)) {
				char* functionAnnotation = strdup(priorTokenSpelling);
				removePrefixFromString(&functionAnnotation, annotation);

				return functionAnnotation;
			}
		}
	}

	return NULL;
}

static void addNewFunctionWithCursor(CXCursor cursor, Function** functionTarget, unsigned* numFunctions) {
	Function newFunction = getFunctionFromCursor(cursor);
	addNewFunction(newFunction, functionTarget, numFunctions);
}

static void addNewFunctionWithCursorAndAnnotation(CXCursor cursor, const char* annotation, Function** functionTarget, unsigned* numFunctions) {
	Function newFunction = getFunctionFromCursor(cursor);

	newFunction.outArraySize = getOutArraySizeFromAnnotation(annotation);

	char** parameterValues = getParameterValuesFromAnnotation(annotation, newFunction.numParameters);
	unsigned inArraySize = getInArraySizeFromAnnotation(annotation);

	for (unsigned i = 0; i < newFunction.numParameters; i++) {
		newFunction.parameters[i].inArraySize = inArraySize;

		if (parameterValues && parameterValues[i]) {
			newFunction.parameters[i].parameterValue = parameterValues[i];
		} else {
			newFunction.parameters[i].parameterValue = NULL;
		}
	}

	addNewFunction(newFunction, functionTarget, numFunctions);
}

static void addNewFunction(Function function, Function** functionTarget, unsigned* numFunctions) {
	if (!arrayContainsFunction(*functionTarget, *numFunctions, function)) {
		*functionTarget = reallocateMemory(*functionTarget, ((*numFunctions) + 1) * sizeof(Function));
		(*functionTarget)[*numFunctions] = function;
		++(*numFunctions);
	}
}

static unsigned getInArraySizeFromAnnotation(const char* annotation) {
	return getArraySizeFromAnnotation(annotation, ANNOTATION_IN_ARRAY_SIZE);
}

static unsigned getOutArraySizeFromAnnotation(const char* annotation) {
	return getArraySizeFromAnnotation(annotation, ANNOTATION_OUT_ARRAY_SIZE);
}

static unsigned getArraySizeFromAnnotation(const char* annotation, const char* whichSize) {
	unsigned arraySize = DEFAULT_ARRAY_SIZE;

	char* annotationWithoutWhitespace = removeWhitespace(annotation);
	removeParametersFromAnnotation(&annotationWithoutWhitespace);
	int annotationLength = strlen(annotationWithoutWhitespace);

	if (annotationLength == 0) {
		return DEFAULT_ARRAY_SIZE;
	} else if (!*annotationWithoutWhitespace == '[' && !annotationWithoutWhitespace[annotationLength - 1] == ']') {
		return DEFAULT_ARRAY_SIZE;
	}

	unsigned numAnnotationParts;
	char** annotationParts;
	char* annotationWithoutBraces = getSubstring(annotationWithoutWhitespace, 2, strlen(annotationWithoutWhitespace) - 2);

	splitStringOnDelimiter(annotationWithoutBraces, ",", &annotationParts, &numAnnotationParts);

	for (unsigned i = 0; i < numAnnotationParts; i++) {
		if (stringStartsWith(annotationParts[i], whichSize)) {
			unsigned numSizeAnnotation;
			char** sizeAnnotation;

			splitStringOnDelimiter(annotationParts[i], "=", &sizeAnnotation, &numSizeAnnotation);
			arraySize = strtol(sizeAnnotation[1], NULL, 10);
		}
	}

	return arraySize == 0 ? DEFAULT_ARRAY_SIZE : arraySize;
}

static char** getParameterValuesFromAnnotation(const char* annotation, const unsigned numParameters) {
	char** parameterValues = allocateMemory(numParameters * sizeof(char*));

	char* annotationWithoutWhitespace = removeWhitespace(annotation);
	removeSizesFromAnnotation(&annotationWithoutWhitespace);
	int annotationLength = strlen(annotationWithoutWhitespace);

	if (annotationLength == 0 || (!*annotationWithoutWhitespace == '(' && !annotationWithoutWhitespace[annotationLength - 1] == ')')) {
		return NULL;
	}

	unsigned numAnnotationParts;
	char** annotationParts;
	char* annotationWithoutBraces = getSubstring(annotationWithoutWhitespace, 2, strlen(annotationWithoutWhitespace) - 2);
	annotationLength = strlen(annotationWithoutBraces);

	unsigned addedParameters = 0;
	int indexOfCurrentComma = 0;
	int indexOfNextComma = 0;

	while (addedParameters < numParameters) {
		if (indexOfCurrentComma >= annotationLength) {
			parameterValues[addedParameters] = NULL;
			++addedParameters;
			continue;
		}

		if (stringStartsWith(annotationWithoutBraces + indexOfCurrentComma, "{")) {
			indexOfNextComma = indexOfCurrentComma + getFirstIndexOfChar(annotationWithoutBraces + indexOfCurrentComma, '}') + 1;
		} else {
			indexOfNextComma = indexOfCurrentComma + getFirstIndexOfChar(annotationWithoutBraces + indexOfCurrentComma, ',');
		}

		if (indexOfNextComma < indexOfCurrentComma) {
			indexOfNextComma = annotationLength;
		}

		char* currentParameter = getSubstring(annotationWithoutBraces, indexOfCurrentComma + 1, indexOfNextComma - indexOfCurrentComma);

		if (!stringsEqual(currentParameter, ANNOTATION_SYMBOLIC)) {
			parameterValues[addedParameters] = currentParameter;
		} else {
			parameterValues[addedParameters] = NULL;
		}

		++addedParameters;
		indexOfCurrentComma = indexOfNextComma + 1; // Plus 1 to skip the comma.
	}

	return parameterValues;
}

static void removeSizesFromAnnotation(char** annotation) {
	const int indexOfClosingBracket = getFirstIndexOfChar(*annotation, ']');

	if (indexOfClosingBracket < 0) {
		return;
	}

	const int newLength = strlen(*annotation) - indexOfClosingBracket - 1;
	*annotation = getSubstring(*annotation, indexOfClosingBracket + 2, newLength);
}

static void removeParametersFromAnnotation(char** annotation) {
	const int indexOfOpeningBrace = getFirstIndexOfChar(*annotation, '(');

	if (indexOfOpeningBrace < 0) {
		return;
	}

	*annotation = getSubstring(*annotation, 1, indexOfOpeningBrace);
}

static Function getFunctionFromCursor(CXCursor cursor) {
	Function function;

	const char* functionReturnType = clang_getCString(clang_getTypeSpelling(clang_getCursorResultType(cursor)));
	const char* functionName = clang_getCString(clang_getCursorSpelling(cursor));
	int numParameters = clang_Cursor_getNumArguments(cursor);

	function.returnType = strdup(functionReturnType);
	function.name = strdup(functionName);
	function.numParameters = numParameters;
	function.parameters = getParametersFromCursor(cursor, numParameters);

	return function;
}

static Parameter* getParametersFromCursor(CXCursor cursor, unsigned numParameters) {
	Parameter* parameters = allocateMemory(numParameters * sizeof(Parameter));

	for (int parameterIndex = 0; parameterIndex < numParameters; parameterIndex++) {
		CXCursor parameterCursor = clang_Cursor_getArgument(cursor, parameterIndex);
		parameters[parameterIndex] = getParameterFromCursor(parameterCursor);
	}

	return parameters;
}

static Parameter getParameterFromCursor(CXCursor cursor) {
	Parameter parameter;

	const char* parameterType = clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor)));
	const char* parameterName = clang_getCString(clang_getCursorSpelling(cursor));

	parameter.parameterType = strdup(parameterType);
	parameter.parameterName = strdup(parameterName);

	return parameter;
}
