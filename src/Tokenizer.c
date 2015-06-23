#include <clang-c/Index.h>

#include "headers/Tokenizer.h"
#include "headers/ClangHelper.h"

void getTokensFromSourceFile(const char* sourceFile, CXToken** tokenTarget, unsigned* numTokens) {
	CXTranslationUnit translationUnit = getTranslationUnitFromSourceFile(sourceFile);
	CXSourceRange sourceRange = getSourceRangeFromTranslationUnit(translationUnit);

	clang_tokenize(translationUnit, sourceRange, tokenTarget, numTokens);

	clang_disposeTranslationUnit(translationUnit);
}
