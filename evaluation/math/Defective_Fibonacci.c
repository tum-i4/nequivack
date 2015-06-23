// @klee
unsigned long fibonacci(unsigned short n) {
	if (n == 0) {
		return 2;
	}

	if (n == 1) {
		return 1;
	}

	return fibonacci(n - 1) + fibonacci(n - 2);
}
