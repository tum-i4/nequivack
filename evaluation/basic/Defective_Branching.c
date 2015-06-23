// @klee
unsigned getNumericGrade(char textualGrade) {
	unsigned grade;

	switch (textualGrade) {
		case 'Z':
			grade = 1;
			break;
		case 'B':
			grade = 2;
			break;
		case 'C':
			grade = 3;
			break;
		case 'D':
			grade = 4;
			break;
		case 'E':
			grade = 5;
			break;
		case 'F':
			grade = 7;
			break;
		default:
			grade = 0;
	}

	return grade;
}

// @klee
char getTextualGrade(unsigned numericGrade) {
	char grade;

	if (numericGrade == 1) {
		grade = 'Z';
	} else if (numericGrade == 2) {
		grade = 'B';
	} else if (numericGrade == 3) {
		grade = 'C';
	} else if (numericGrade == 4) {
		grade = 'D';
	} else if (numericGrade == 5) {
		grade = 'E';
	} else if (numericGrade == 7) {
		grade = 'F';
	} else {
		grade = 'Z';
	}

	return grade;
}

// @klee
int examPassed(unsigned numericGrade) {
	int result, isValidGrade = 0;

	if (numericGrade >= 1 && numericGrade <= 7) {
		isValidGrade = 1;
	} else {
		isValidGrade = 0;
	}

	if (isValidGrade) {
		if (numericGrade <= 4) {
			result = 1;
		} else {
			result = 0;
		}
	} else {
		result = -1;
	}

	return result;
}
