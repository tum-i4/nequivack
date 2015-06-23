#include <stdlib.h>
#include <limits.h>

// @klee
short divide(short a, short b) {
	if (a == SHRT_MIN || b == SHRT_MIN || b == 0) {
		// problematic boundary cases for absolute value
		// or division by zero
		return 0;
	}

	short result = 0;
	short dividend = abs(a);
	short divisor = abs(b);

	while (dividend >= divisor) {
		dividend -= divisor;
		result++;
	}

	return (a < 0 && b < 0) || (a > 0 && b > 0) ? result : -result;
}
