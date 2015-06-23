#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#include "headers/LibraryHelper.h"
#include "headers/String.h"

bool stringsEqual(char* str1, char* str2) {
	int length1 = strlen(str1);
	int length2 = strlen(str2);

	if ((length1 == length2) && !strncmp(str1, str2, length1)) {
		return true;
	}

	return false;
}

bool constStringsEqual(const char* constStr1, const char* constStr2) {
	char* str1 = strdup(constStr1);
	char* str2 = strdup(constStr2);

	return stringsEqual(str1, str2);
}

char* getStringFromInt(const int number) {
	const int intLength = snprintf(NULL, 0, "%d", number);
	assert(intLength > 0);

	char* intString = allocateMemory(intLength + 1);
	const int stringLength = snprintf(intString, intLength + 1, "%d", number);

	assert(intString[intLength] == '\0');
	assert(stringLength == intLength);

	return intString;
}

void splitStringOnDelimiter(char* string, const char* delimiter, char*** tokenTarget, unsigned* numTokens) {
	*numTokens = 0;
	*tokenTarget = allocateMemory(sizeof(char*));

	char* token = strtok(string, delimiter);

	while (token != NULL) {
		*tokenTarget = reallocateMemory(*tokenTarget, ((*numTokens) + 1) * sizeof(char*));
		(*tokenTarget)[*numTokens] = token;
		++(*numTokens);

		// Get next token.
		token = strtok(NULL, delimiter);
	}
}

char* concatenateStrings(const int numStrings, ...) {
	va_list inputStrings;

	// Calculate length of concatenated string.
	int concatenatedLength = 1; // NUL

	va_start(inputStrings, numStrings);
	for (unsigned i = 0; i < numStrings; i++) {
		concatenatedLength += strlen(va_arg(inputStrings, char*));
	}
	va_end(inputStrings);

	// Concatenate strings.
	char* concatenatedString = allocateMemory(concatenatedLength);
	int stringOffset = 0;

	va_start(inputStrings, numStrings);
	for (unsigned i = 0; i < numStrings; i++) {
		char* currentString = va_arg(inputStrings, char*);
		strcpy(concatenatedString + stringOffset, currentString);
		stringOffset += strlen(currentString);
	}
	va_end(inputStrings);

	return concatenatedString;
}

void removePrefixFromStrings(char** strings, const unsigned numStrings, const char* prefix) {
	for (unsigned i = 0; i < numStrings; i++) {
		removePrefixFromString(&(strings[i]), prefix);
	}
}

void removePrefixFromString(char** string, const char* prefix) {
	if (!stringStartsWith(*string, prefix)) {
		return;
	}

	int stringLength = strlen(*string);
	int prefixLength = strlen(prefix);
	int resultingStringLength = stringLength - prefixLength;

	char* resultingString = allocateMemory(resultingStringLength + 1);
	memcpy(resultingString, &(*string)[prefixLength], resultingStringLength);
	resultingString[resultingStringLength] = '\0';

	*string = resultingString;
}

void removeSuffixFromString(char** string, const char* suffix) {
	if (!stringEndsWith(*string, suffix)) {
		return;
	}

	int stringLength = strlen(*string);
	int sufixLength = strlen(suffix);
	int resultingStringLength = stringLength - sufixLength;

	char* resultingString = allocateMemory(resultingStringLength + 1);
	memcpy(resultingString, *string, resultingStringLength);
	resultingString[resultingStringLength] = '\0';

	*string = resultingString;
}

void removePrefixedStrings(char*** strings, unsigned* numStrings, const char* prefix) {
	char** stringsWithoutPrefix = allocateMemory(sizeof(char*));
	unsigned numStringsWithoutPrefix = 0;

	for (unsigned i = 0; i < *numStrings; i++) {
		if (!stringStartsWith((*strings)[i], prefix)) {
			++numStringsWithoutPrefix;
			stringsWithoutPrefix = reallocateMemory(stringsWithoutPrefix, numStringsWithoutPrefix * sizeof(char*));
			stringsWithoutPrefix[numStringsWithoutPrefix - 1] = (*strings)[i];
		}
	}

	(*numStrings) = numStringsWithoutPrefix;
	(*strings) = stringsWithoutPrefix;
}

int getFirstIndexOfChar(const char* string, const char c) {
	const char *pointerToC = strchr(string, c);

	if (pointerToC) {
		return pointerToC - string;
	}

	return -1;
}

bool stringStartsWith(const char* string, const char* prefix) {
	int stringLength = strlen(string);
	int prefixLength = strlen(prefix);

	if (stringLength < prefixLength) {
		return false;
	}

	return strncmp(string, prefix, prefixLength) == 0;
}

bool stringEndsWith(const char* string, const char* suffix) {
	int stringLength = strlen(string);
	int suffixLength = strlen(suffix);

	if (stringLength < suffixLength) {
		return false;
	}

	return strncmp(string + stringLength - suffixLength, suffix, suffixLength) == 0;
}

char* getSubstring(const char* string, const int position, const int length) {
	if (position <= 0) {
		return NULL;
	}

	char* substring = allocateMemory(length + 1);

	for (unsigned i = 0; i < length; i++) {
		substring[i] = string[position - 1 + i];
	}

	substring[length] = '\0';

	return substring;
}

void insertSubstringAtPosition(char** string, const char* substring, const int position) {
	char* partBefore = getSubstring(*string, 1, position - 1);
	char* partAfter = getSubstring(*string, position, strlen(*string) - position + 1);
	char* newString = concatenateStrings(3, partBefore, substring, partAfter);

	free(partBefore);
	free(partAfter);
	free(*string);

	*string = newString;
}

bool stringContainsSubtring(const char* string, const char* substring) {
	if (strstr(string, substring) != NULL) {
		return true;
	}

	return false;
}

char* removeWhitespace(const char* string) {
	return replaceInString(string, " ", "");
}

/* Function taken from http://creativeandcritical.net/str-replace-c */
char* replaceInString(const char* str, const char* old, const char* new) {

	/* Adjust each of the below values to suit your needs. */

	/* Increment positions cache size initially by this number. */
	size_t cache_sz_inc = 16;
	/* Thereafter, each time capacity needs to be increased,
	 * multiply the increment by this factor. */
	const size_t cache_sz_inc_factor = 2;
	/* But never increment capacity by more than this number. */
	const size_t cache_sz_inc_max = 1048576;

	char *pret, *ret = NULL;
	const char *pstr2, *pstr = str;
	size_t i, count = 0;
	ptrdiff_t *pos_cache = NULL;
	size_t cache_sz = 0;
	size_t cpylen, orglen, retlen, newlen, oldlen = strlen(old);

	/* Find all matches and cache their positions. */
	while ((pstr2 = strstr(pstr, old)) != NULL) {
		count++;

		/* Increase the cache size when necessary. */
		if (cache_sz < count) {
			cache_sz += cache_sz_inc;
			pos_cache = realloc(pos_cache, sizeof(*pos_cache) * cache_sz);
			if (pos_cache == NULL) {
				goto end_repl_str;
			}
			cache_sz_inc *= cache_sz_inc_factor;
			if (cache_sz_inc > cache_sz_inc_max) {
				cache_sz_inc = cache_sz_inc_max;
			}
		}

		pos_cache[count - 1] = pstr2 - str;
		pstr = pstr2 + oldlen;
	}

	orglen = pstr - str + strlen(pstr);

	/* Allocate memory for the post-replacement string. */
	if (count > 0) {
		newlen = strlen(new);
		retlen = orglen + (newlen - oldlen) * count;
	} else
		retlen = orglen;
	ret = malloc(retlen + 1);
	if (ret == NULL) {
		goto end_repl_str;
	}

	if (count == 0) {
		/* If no matches, then just duplicate the string. */
		strcpy(ret, str);
	} else {
		/* Otherwise, duplicate the string whilst performing
		 * the replacements using the position cache. */
		pret = ret;
		memcpy(pret, str, pos_cache[0]);
		pret += pos_cache[0];
		for (i = 0; i < count; i++) {
			memcpy(pret, new, newlen);
			pret += newlen;
			pstr = str + pos_cache[i] + oldlen;
			cpylen = (i == count - 1 ? orglen : pos_cache[i + 1]) - pos_cache[i] - oldlen;
			memcpy(pret, pstr, cpylen);
			pret += cpylen;
		}
		ret[retlen] = '\0';
	}

	end_repl_str:
	/* Free the cache and return the post-replacement string,
	 * which will be NULL in the event of an error. */
	free(pos_cache);
	return ret;
}
