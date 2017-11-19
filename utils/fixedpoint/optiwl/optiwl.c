/********************************************
  optiwl.c

	input: file which contains float numbers
	output: optimal Integer Word Length (IWL)
*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[]) {
	
	if(argc < 2) {
		printf("usage: %s <input>\n\n", argv[0]);
		printf("arguments:\n");
		printf("<input>: matrix file (*.mat)\n");
		printf("         e.g. test.mat\n");
		printf("         2 3\n");
		printf("         a11 a12 a13\n");
		printf("         a21 a22 a23\n");
		exit(1);
	}
	
	FILE *fd;
	int max = 0;
	int iwl = 0;
	int N, rows, cols;
	
	fd = fopen(argv[1], "r");
	fscanf(fd, "%d %d\n", &rows, &cols);
	N = rows * cols;

	for(int i = 0; i < N; i++) {
		float f;
		int n;

		fscanf(fd, "%f", &f);
		n = f;

		if(n < 0) n *= (-1);

		if(max < n) max = n;
	}

	fclose(fd);

	if(max) iwl = log2(max) + 1;
	
	printf("shape: (%d, %d)\n", rows, cols);
	printf("max unsigned integer:  %d\n", max);
	printf("optimal IWL: %d\n", iwl);

	return 0;
}
