struct ComplexNumber {
	int real, img;
};

// @klee
struct ComplexNumber add(struct ComplexNumber a, struct ComplexNumber b) {
	struct ComplexNumber c;

	c.real = a.real + b.real;
	c.img = a.img + b.img;

	return c;
}

// @klee
struct ComplexNumber sub(struct ComplexNumber a, struct ComplexNumber b) {
	struct ComplexNumber c;

	c.real = a.real - b.real;
	c.img = a.img - b.img;

	return c;
}

// @klee
struct ComplexNumber mul(struct ComplexNumber a, struct ComplexNumber b) {
	struct ComplexNumber c;

	c.real = a.real * b.real - a.img * b.img;
	c.img = a.img * b.real + a.real * b.img;

	return c;
}
