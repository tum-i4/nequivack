#define CHARS_IN_ALPHABET 26

// @klee
int getPositionInAlphabet_DoWhile(char c) {
	int offset = -1;

	do {
		offset++;
	} while (offset < CHARS_IN_ALPHABET && 'a' + offset != c);

	if (offset == CHARS_IN_ALPHABET) {
		return -1;
	}

	return offset;
}

// @klee
int getPositionInAlphabet_While(char c) {
	int offset = 0;

	while (offset < CHARS_IN_ALPHABET) {
		if ('a' + offset == c) {
			break;
		}

		offset++;
	}

	if (offset == CHARS_IN_ALPHABET) {
		return -1;
	}

	return offset;
}

// @klee
int getPositionInAlphabet_For(char c) {
	for (int offset = 0; offset < CHARS_IN_ALPHABET; offset++) {
		if ('a' + offset != c) {
			continue;
		}

		return offset;
	}

	return -1;
}
