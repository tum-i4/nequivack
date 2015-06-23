// @klee[inArraySize=6,outArraySize=6](sym,6)
int* sort(int a[], int n) {
	int i, j, p, t;

	if (n < 2)
		return a;

	p = a[n / 2];

	for (i = 0, j = n - 1;; i++, j--) {
		while (a[i] < p) // < ascending
			i++;

		while (p < a[j]) // < ascending
			j--;

		if (i >= j)
			break;

		t = a[i];
		a[i] = a[j];
		a[j] = t;
	}

	sort(a, i);
	sort(a + i, n - i);

	return a;
}
