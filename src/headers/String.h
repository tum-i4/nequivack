#ifndef STRING_H_
#define STRING_H_

#include <stdbool.h>

bool stringsEqual(char*, char*);
bool constStringsEqual(const char*, const char*);

char* getStringFromInt(const int);

void splitStringOnDelimiter(char*, const char*, char***, unsigned*);

char* concatenateStrings(const int, ...);

void removePrefixFromStrings(char**, const unsigned, const char*);
void removePrefixFromString(char**, const char*);

void removeSuffixFromString(char**, const char*);

void removePrefixedStrings(char***, unsigned*, const char*);

int getFirstIndexOfChar(const char*, const char);

bool stringStartsWith(const char*, const char*);
bool stringEndsWith(const char*, const char*);

char* getSubstring(const char*, const int, const int);
void insertSubstringAtPosition(char**, const char*, const int);

bool stringContainsSubtring(const char*, const char*);

char* removeWhitespace(const char*);

char* replaceInString(const char*, const char*, const char*);

#endif
