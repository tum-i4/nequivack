// @klee
unsigned long sum(unsigned int n) {
	unsigned long sum = 0;

	for (unsigned long i = 1; i <= n; i++) {
		sum += i;
	}

	return sum;
}
