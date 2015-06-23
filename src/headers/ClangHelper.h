#ifndef CLANGHELPER_H_
#define CLANGHELPER_H_

#include <stdbool.h>

#include <clang-c/Index.h>

CXTranslationUnit getTranslationUnitFromSourceFile(const char*);
CXSourceRange getSourceRangeFromTranslationUnit(CXTranslationUnit);

#endif
