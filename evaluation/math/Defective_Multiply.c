#include <stdlib.h>
#include <limits.h>

// @klee
long multiply(short a, short b) {
	if (a == SHRT_MIN || b == SHRT_MIN) {
		// problematic boundary cases for absolute value
		return 0;
	}

	long result = 0;

	for (short i = abs(b); i > 0; i--) {
		result += abs(a);
	}

	return (a < 0 && b < 0) || (a > 0 && b > 0) ? -result : result;
}
