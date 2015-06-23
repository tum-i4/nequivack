// @klee
unsigned short gcd(unsigned short a, unsigned short b) {
	unsigned short c;

	while (a != 0) {
		c = a;
		a = b % a;
		b = c;
	}

	return 1;
}
