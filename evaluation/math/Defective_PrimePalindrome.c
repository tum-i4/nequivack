#include <stdio.h>
#include <stdbool.h>

unsigned int sqrt32(unsigned long);

// @klee
unsigned long nextPrimePalindrome(unsigned long n) {
	if (n > 18446744073709500000) {
		// to limit big numbers
		return 0;
	}

	unsigned long t, r = 0, c, d;

	while (true) {
		n--;
		t = n;

		/* Calculating reverse of number */

		while (t) {
			r *= 10; /* Compound assignment operator r*=10 => r=r*10 */
			r += t % 10;
			t /= 10;
		}

		/* if reverse equals original then it is palindrome */

		if (r == n) {
			d = sqrt32(n);

			/* Checking prime */

			for (c = 2; c <= d; c++) {
				if (n % c == 0)
					break;
			}
			if (c == d + 1)
				break;
		}
		r = 0;
	}

	return n;
}

unsigned int sqrt32(unsigned long n) {
	// Taken from http://www.codecodex.com/wiki/Calculate_an_integer_square_root#C
	unsigned int c = 0x8000;
	unsigned int g = 0x8000;

	for (;;) {
		if (g * g > n)
			g ^= c;

		c >>= 1;

		if (c == 0)
			return g;

		g |= c;
	}

	return 0;
}
