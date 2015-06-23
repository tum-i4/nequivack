#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include "headers/File.h"
#include "headers/LibraryHelper.h"

char* getFileNameFromPath(const char* path) {
	char* pathCopy = strdup(path);
	return basename(pathCopy);
}

void readFileToStringArray(const char* file, char*** strings, unsigned* numStrings) {
	*numStrings = getLineCountOfFile(file);
	*strings = allocateMemory((*numStrings) * sizeof(char*));

	FILE* filePtr = openFileForReading(file);
	unsigned lineNumber = 0, lineLength;
	char lineBuffer[BUFSIZ];

	while (fgets(lineBuffer, sizeof(lineBuffer), filePtr)) {
		lineLength = strlen(lineBuffer);
		(*strings)[lineNumber] = allocateMemory(lineLength + 1);

		strncpy((*strings)[lineNumber], lineBuffer, lineLength + 1);

		++lineNumber;
	}

	fclose(filePtr);
}

void writeStringArrayToFile(const char* file, char** strings, const unsigned numStrings) {
	FILE* targetFile = openFileForWriting(file);

	for (unsigned i = 0; i < numStrings; i++) {
		fputs(strings[i], targetFile);
	}

	fclose(targetFile);
}

void writeStringArrayToFileWithSuffix(const char* file, char** strings, const unsigned numStrings, const char* suffix) {
	FILE* targetFile = openFileForWriting(file);

	for (unsigned i = 0; i < numStrings; i++) {
		fputs(strings[i], targetFile);
		fputs(suffix, targetFile);
	}

	fclose(targetFile);
}

static unsigned getLineCountOfFile(const char* file) {
	FILE* filePtr = openFileForReading(file);
	unsigned lineCount = 0;
	char lineBuffer[BUFSIZ];

	while (fgets(lineBuffer, sizeof(lineBuffer), filePtr)) {
		++lineCount;
	}

	fclose(filePtr);

	return lineCount;
}

static FILE* openFileForReading(const char* file) {
	return openFile(file, "r");
}

static FILE* openFileForWriting(const char* file) {
	return openFile(file, "w");
}

static FILE* openFile(const char* file, const char* opentype) {
	FILE* filePtr = fopen(file, opentype);

	checkFileOpening(filePtr);

	return filePtr;
}

static void checkFileOpening(const FILE* pointer) {
	if (!pointer) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
}

void removeFile(const char* path) {
	char* removeAssertionErrorFileCommand = allocateMemory(3 + strlen(path) + 1);
	sprintf(removeAssertionErrorFileCommand, "rm %s", path);

	system(removeAssertionErrorFileCommand);
}
