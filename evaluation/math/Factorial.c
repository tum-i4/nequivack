// @klee
unsigned long factorial(char n) {
	if (n <= 1) {
		return 1;
	} else if (n > 20) {
		// reached limit of ULONG
		return 0;
	}

	return n * factorial(n - 1);
}
