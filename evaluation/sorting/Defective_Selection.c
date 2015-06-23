// @klee[inArraySize=6,outArraySize=6](sym,6)
int* sort(int a[], int array_size) {
	int i;

	for (i = 0; i < array_size - 1; ++i) {
		int j, min, temp;
		min = i;

		for (j = i + 1; j < array_size; ++j) {
			if (a[j] > a[min]) { // Ascending with <
				min = j;
			}
		}

		temp = a[i];
		a[i] = a[min];
		a[min] = temp;
	}

	return a;
}
