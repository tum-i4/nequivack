#ifndef STRUCTEXTRACTOR_H_
#define STRUCTEXTRACTOR_H_

#include <clang-c/Index.h>

#include "Structure.h"

void getStructs(const char*, Structure**, unsigned*);
void getStructsFromTokens(const char*, Structure**, unsigned*, CXToken*, unsigned);
static void addNewStructWithCursor(CXCursor, Structure**, unsigned*);
static enum CXChildVisitResult visitStructMembers(CXCursor, CXCursor, CXClientData);

#endif
