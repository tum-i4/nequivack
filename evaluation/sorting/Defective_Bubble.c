// @klee[inArraySize=6,outArraySize=6](sym,6)
int* sort(int a[], int array_size) {
	int i, j, temp;

	for (i = 0; i < (array_size - 1); ++i) {
		for (j = 0; j < array_size - 1 - i; ++j) {
			if (a[j] < a[j + 1]) { // Ascending with >
				temp = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp;
			}
		}
	}

	return a;
}
