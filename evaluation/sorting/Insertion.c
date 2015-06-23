// @klee[inArraySize=6,outArraySize=6](sym,6)
int* sort(int a[], int n) {
	int i, j, t;

	for (i = 1; i < n; i++) {
		t = a[i];

		for (j = i; j > 0 && t < a[j - 1]; j--) { // Ascending with <
			a[j] = a[j - 1];
		}

		a[j] = t;
	}

	return a;
}
