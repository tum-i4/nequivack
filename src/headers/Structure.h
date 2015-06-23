#ifndef STRUCTURE_H_
#define STRUCTURE_H_

#include <stdbool.h>

#include "Parameter.h"

typedef struct Structure {
	char* name;
	Parameter* members;
	unsigned numMembers;
} Structure;

bool structsEqual(Structure, Structure);
void generateCodeForStructs(Structure*, unsigned, char***, unsigned*);
void generateCodeForStruct(Structure, char***, unsigned*);

Structure* getStructWithType(char*, Structure*, const unsigned);
char* getBaseNameOfStruct(Structure);

#endif
