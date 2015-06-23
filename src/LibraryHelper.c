#include <stdio.h>
#include <stdlib.h>

#include "headers/LibraryHelper.h"

void* allocateMemory(size_t size) {
	return reallocateMemory(NULL, size);
}

void* reallocateMemory(void* pointer, size_t size) {
	void* reallocatedPointer = realloc(pointer, size);

	checkAllocation(reallocatedPointer);

	return reallocatedPointer;
}

static void checkAllocation(const void* pointer) {
	if (!pointer) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}
}

