#include <stdlib.h>
#include <limits.h>

// @klee
long power(short a, short b) {
	if (a == SHRT_MIN || b == SHRT_MIN) {
		// problematic boundary cases for absolute value
		return 0;
	} else if (a == 1 || b == 0) {
		return 1;
	} else if (a == 0 || b < 0) {
		return 0;
	}

	long result = 1;

	for (short i = abs(b); i > 0; i--) {
		result *= abs(a);
	}

	return !(b % 2 == 0) && a < 0 ? -result : result;
}
