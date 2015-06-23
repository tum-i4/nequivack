#include <stdio.h>
#include <stdlib.h>

#include <clang-c/Index.h>

#include "headers/ClangHelper.h"

CXTranslationUnit getTranslationUnitFromSourceFile(const char* sourceFile) {
	const char* clangArgs[] = {"-x", "c"};
	int numArgs = 2;

	CXIndex index = clang_createIndex(0, 1);
	CXTranslationUnit translationUnit = clang_parseTranslationUnit(index, sourceFile, clangArgs, numArgs, NULL, 0, CXTranslationUnit_None);

	if (!translationUnit) {
		printf("ERROR parsing translation unit.\n");
		exit(1);
	}

	clang_disposeIndex(index);

	return translationUnit;
}

CXSourceRange getSourceRangeFromTranslationUnit(CXTranslationUnit translationUnit) {
	CXCursor startCursor = clang_getTranslationUnitCursor(translationUnit);
	return clang_getCursorExtent(startCursor);
}
