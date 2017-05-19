#include <stdio.h>
#include <string.h>
#define INPUT_LENGTH 784
#define HL_LENGTH 512
#define OUTPUT_LENGTH 10
#define TOKEN_LENGTH 25 // pos num: 24, neg num: 25
#define BUFFER_SIZE 1024

long double X[INPUT_LENGTH + 1];

long double W1[INPUT_LENGTH][HL_LENGTH];
long double W2[HL_LENGTH][HL_LENGTH];
long double W3[HL_LENGTH][HL_LENGTH];
long double W4[HL_LENGTH][HL_LENGTH];
long double W5[HL_LENGTH][OUTPUT_LENGTH];

long double B1[HL_LENGTH];
long double B2[HL_LENGTH];
long double B3[HL_LENGTH];
long double B4[HL_LENGTH];
long double B5[OUTPUT_LENGTH];


long double test[10][1024];
/*
int saveToken(char* token, int idx) {
	printf("token: ");
	long double val;
	sscanf(token, "%Lf", &val);
	printf("%.12Lf\n", val);
	test[idx] = val;
	return 0;
}
*/

void passIL(int x[INPUT_LENGTH], int w[INPUT_LENGTH][HL_LENGTH], int b[HL_LENGTH], int result[HL_LENGTH]) {
	
	for(int n = 0; n < HL_LENGTH; n++) result[n] = b[n]; // copy b to result
	
	for(int m = 0; m < INPUT_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) result[n] += (x[m] * w[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
		if(result[n] < 0) result[n] = 0;
}

void passHL(int x[HL_LENGTH], int w[HL_LENGTH][HL_LENGTH], int b[HL_LENGTH], int result[HL_LENGTH]) {
	
	for(int n = 0; n < HL_LENGTH; n++) result[n] = b[n]; // copy b to result
	
	for(int m = 0; m < HL_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) result[n] += (x[m] * w[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
		if(result[n] < 0) result[n] = 0;

}

void passOL(int x[HL_LENGTH], int w[HL_LENGTH][OUTPUT_LENGTH], int b[OUTPUT_LENGTH], int result[OUTPUT_LENGTH]) {
	
	for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] = b[n]; // copy b to result
	
	for(int m = 0; m < HL_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] += (x[m] * w[m][n]);

	for(int n = 0; n < OUTPUT_LENGTH; n++) // reLU
		if(result[n] < 0) result[n] = 0;

}

void loadW(char* filename, long double mat[][]) {
	char line[BUFFER_SIZE];
	FILE *f;
	f = fopen(filename, "r");
	
	char static token[TOKEN_LENGTH];
	int token_cursor = 0;
	int token_cnt = 0;
	int rows;
	int cols;
	fgets(line, sizeof(line), f);
	sscanf(line, "%d %d", &rows, &cols);
	printf("(%d, %d)\n", rows, cols);
	while(fgets(line, sizeof(line), f)) {
		puts("tokens:");
		int off = 0;

		// read token
		for(int i = 0; i < BUFFER_SIZE; i++) {
			if(off) break;
			switch(line[i]) {
			case ' ':
				// save token
				token[token_cursor] = '\0';
				token_cursor = 0;
				puts(token);
				// saveToken(token, token_cnt++);
			break;

			case '\n':
			case '\0':
				if ((token_cursor == 24 && token[0] != '-') || (token_cursor == 25 && token[0] == '-')) {
					// save token
					token[token_cursor] = '\0';
					token_cursor = 0;
					puts(token);
					// saveToken(token, token_cnt++);
				}

				if (line[i] == '\0') {
					off = 1;
					printf("i: %d, token_cursor: %d, off: %d\n", i, token_cursor, off);
				}
			break;

			default:
				token[token_cursor++] = line[i];
			break;
			}
		}

		puts("===============================");
	}
	fclose(f);
}

int main() {

	loadW("test.w", test);
	puts("print W1...");
	for (int i = 0 ; i < 1024; i++) {
		//printf("%.12Lf ", test[i]);		
	}
	puts("");
	return 0;
}
