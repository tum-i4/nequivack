#ifndef LIBRARYHELPER_H_
#define LIBRARYHELPER_H_

#include <stdlib.h>

void* allocateMemory(size_t);
void* reallocateMemory(void*, size_t);
static void checkAllocation(const void*);

#endif
