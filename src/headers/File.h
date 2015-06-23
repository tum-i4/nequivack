#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>

char* getFileNameFromPath(const char*);

void readFileToStringArray(const char*, char***, unsigned*);
void writeStringArrayToFile(const char*, char**, const unsigned);
void writeStringArrayToFileWithSuffix(const char*, char**, const unsigned, const char*);

static unsigned getLineCountOfFile(const char*);
static FILE* openFileForReading(const char*);
static FILE* openFileForWriting(const char*);
static FILE* openFile(const char*, const char*);
static void checkFileOpening(const FILE*);

void removeFile(const char*);

#endif
