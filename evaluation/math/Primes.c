#include <stdlib.h>
#include <stdbool.h>

// @klee
unsigned short getNextPrime(unsigned short);
bool isPrime(unsigned short);

unsigned short getNextPrime(unsigned short n) {
	if (n > 65521) {
		// Biggest prime in USHORT
		return 0;
	}

	// Refer to http://primerecords.dk/primegaps/maximal.htm
	unsigned short primeGapUpperBound = 4680;

	for (unsigned short i = n; i < n + primeGapUpperBound; i++) {
		if (isPrime(i)) {
			return i;
		}
	}

	return 0;
}

bool isPrime(unsigned short number) {
	if (number <= 1) {
		return false;
	}

	for (unsigned short i = 2; i * i <= number; i++) {
		if (number % i == 0) {
			return false;
		}
	}

	return true;
}
