#include <stdlib.h>

void merge(int a[], int n, int m) {
	int i, j, k;
	int *x = malloc(n * sizeof(int));

	for (i = 0, j = m, k = 0; k < n; k++) {
		x[k] = j == n ? a[i++] : i == m ? a[j++] : a[j] < a[i] ? a[j++] : a[i++]; // < ascending
	}

	for (i = 0; i < n; i++) {
		a[i] = x[i];
	}

	free(x);
}

// @klee[inArraySize=6,outArraySize=6](sym,6)
int* sort(int a[], int n) {
	if (n < 2)
		return a;

	int m = n / 2;

	sort(a, m);
	sort(a + m, n - m);

	merge(a, n, m);

	return a;
}
