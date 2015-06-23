#ifndef FUNCTIONEXTRACTOR_H_
#define FUNCTIONEXTRACTOR_H_

#include <clang-c/Index.h>

#include "Function.h"

void getFunctions(const char*, Function**, unsigned*);
void getAnnotatedFunctions(const char*, const char*, Function**, unsigned*);

void getFunctionsFromTokens(const char*, Function**, unsigned*, CXToken*, unsigned);
void getAnnotatedFunctionsFromTokens(const char*, const char*, Function**, unsigned*, CXToken*, unsigned);

static char* getAnnotationIfCorrect(CXTranslationUnit, const char*, CXToken* tokens, unsigned);
static void addNewFunctionWithCursor(CXCursor, Function**, unsigned*);
static void addNewFunctionWithCursorAndAnnotation(CXCursor, const char*, Function**, unsigned*);
static void addNewFunction(Function, Function**, unsigned*);

static unsigned getInArraySizeFromAnnotation(const char*);
static unsigned getOutArraySizeFromAnnotation(const char*);
static unsigned getArraySizeFromAnnotation(const char*, const char*);
static char** getParameterValuesFromAnnotation(const char*, const unsigned);
static void removeSizesFromAnnotation(char**);
static void removeParametersFromAnnotation(char**);

static Function getFunctionFromCursor(CXCursor);
static Parameter* getParametersFromCursor(CXCursor, unsigned);
static Parameter getParameterFromCursor(CXCursor);

#endif
