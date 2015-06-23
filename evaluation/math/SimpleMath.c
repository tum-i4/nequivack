// @klee
char sign(int a) {
	if (a < 0) {
		return '-';
	}

	return '+';
}

// @klee
int absolute(int a) {
	if (a < 0) {
		a = -a;
	}

	return a;
}

// @klee
int isEven(int a) {
	return a % 2 == 0;
}

// @klee
int add(int a, int b) {
	return a + b;
}

// @klee
int subtract(int a, int b) {
	return add(a, -b);
}
