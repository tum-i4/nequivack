#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <clang-c/Index.h>

#include "headers/Array.h"
#include "headers/ClangHelper.h"
#include "headers/LibraryHelper.h"
#include "headers/Structure.h"
#include "headers/StructExtractor.h"
#include "headers/Tokenizer.h"

void getStructs(const char* sourceFile, Structure** structsTarget, unsigned* numStructs) {
	unsigned numTokens;
	CXToken* tokens;
	getTokensFromSourceFile(sourceFile, &tokens, &numTokens);

	getStructsFromTokens(sourceFile, structsTarget, numStructs, tokens, numTokens);
}

void getStructsFromTokens(const char* sourceFile, Structure** structsTarget, unsigned* numStructs, CXToken* tokens, unsigned numTokens) {
	CXTranslationUnit translationUnit = getTranslationUnitFromSourceFile(sourceFile);

	*numStructs = 0;
	*structsTarget = allocateMemory(sizeof(Structure));

	CXCursor* cursors = allocateMemory(numTokens * sizeof(CXCursor));
	clang_annotateTokens(translationUnit, tokens, numTokens, cursors);

	for (unsigned index = 0; index < numTokens; index++) {
		CXCursor cursor = cursors[index];
		enum CXCursorKind tokenCursorKind = clang_getCursorKind(cursor);

		if (tokenCursorKind == CXCursor_StructDecl) {
			addNewStructWithCursor(cursor, structsTarget, numStructs);
		}
	}

	clang_disposeTranslationUnit(translationUnit);
}

static void addNewStructWithCursor(CXCursor cursor, Structure** structsTarget, unsigned* numStructs) {
	const char* structName = clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor)));

	Structure* structure = allocateMemory(sizeof(Structure));
	structure->name = strdup(structName);

	if (arrayContainsStruct(*structsTarget, *numStructs, *structure)) {
		return;
	}

	structure->numMembers = 0;
	structure->members = allocateMemory(sizeof(Parameter));

	clang_visitChildren(cursor, visitStructMembers, (void*) structure);

	*structsTarget = reallocateMemory(*structsTarget, ((*numStructs) + 1) * sizeof(Structure));
	(*structsTarget)[*numStructs] = *structure;
	++(*numStructs);
}

static enum CXChildVisitResult visitStructMembers(CXCursor cursor, CXCursor parent, CXClientData clientData) {
	Structure* structure = (Structure*) clientData;
	const char* memberType = clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor)));
	const char* memberName = clang_getCString(clang_getCursorSpelling(cursor));

	Parameter* member = allocateMemory(sizeof(Parameter));

	member->parameterType = strdup(memberType);
	member->parameterName = strdup(memberName);
	member->parameterValue = NULL;

	structure->members = reallocateMemory(structure->members, ((structure->numMembers) + 1) * sizeof(Parameter));
	structure->members[structure->numMembers] = *member;
	++(structure->numMembers);

	return CXChildVisit_Continue;
}
