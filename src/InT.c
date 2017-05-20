#include <stdio.h>
#include <string.h>
#define INPUT_LENGTH 784
#define HL_LENGTH 512
#define OUTPUT_LENGTH 10
#define TOKEN_LENGTH 24 // pos num: 24, neg num: 25
#define BUFFER_SIZE 1024

long double X[INPUT_LENGTH];

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

long double evalToken(char* token) {
	long double val;
	sscanf(token, "%Lf", &val);
	return val;
}

void passIL(long double x[INPUT_LENGTH], long double w[INPUT_LENGTH][HL_LENGTH], long double b[HL_LENGTH], long double result[HL_LENGTH]) {
	
	for(int n = 0; n < HL_LENGTH; n++) result[n] = b[n]; // copy b to result
	
	for(int m = 0; m < INPUT_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) result[n] += (x[m] * w[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
		if(result[n] < 0) result[n] = 0;
}

void passHL(long double x[HL_LENGTH], long double w[HL_LENGTH][HL_LENGTH], long double b[HL_LENGTH], long double result[HL_LENGTH]) {
	
	for(int n = 0; n < HL_LENGTH; n++) result[n] = b[n]; // copy b to result
	
	for(int m = 0; m < HL_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) result[n] += (x[m] * w[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
		if(result[n] < 0) result[n] = 0;

}

int passOL(long double x[HL_LENGTH], long double w[HL_LENGTH][OUTPUT_LENGTH], long double b[OUTPUT_LENGTH], long double result[OUTPUT_LENGTH]) {
	
	for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] = b[n]; // copy b to result
	
	for(int m = 0; m < HL_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] += (x[m] * w[m][n]);

	long double max = result[0];
	int argmax = 0;
	for(int n = 1; n < OUTPUT_LENGTH; n++) // get max value (softmax part)
		if(result[n] > max) {
			max = result[n];
			argmax = n;
		}

	return argmax;

}

void loadParam(char* filename, long double* mat) {
	char line[BUFFER_SIZE];
	FILE *f;
	
	static char token[TOKEN_LENGTH];
	static long double* ptr = 0;
	int token_cursor = 0;
	int token_cnt = 0;
	int rows;
	int cols;
	
	f = fopen(filename, "r");
	fgets(line, sizeof(line), f);
	sscanf(line, "%d %d", &rows, &cols);
	
	while(fgets(line, sizeof(line), f)) {
		int off = 0;

		// read token
		for(int i = 0; i < BUFFER_SIZE; i++) {
			if(off) break;
			switch(line[i]) {
			case ' ':
				if((token_cursor == TOKEN_LENGTH && token[0] != '-') ||
						(token_cursor == TOKEN_LENGTH + 1 && token[0] == '-')) {
					// save token
					token[token_cursor] = '\0';
					token_cursor = 0;
					*(mat + token_cnt) = evalToken(token);
					token_cnt++;
				}
			break;

			case '\n':
			case '\0':
				if((token_cursor == TOKEN_LENGTH && token[0] != '-') ||
						(token_cursor == TOKEN_LENGTH + 1 && token[0] == '-')) {
					// save token
					token[token_cursor] = '\0';
					token_cursor = 0;
					*(mat + token_cnt) = evalToken(token);
					token_cnt++;
				}

				if(line[i] == '\0') off = 1;
			break;

			default:
				token[token_cursor++] = line[i];
			break;
			}
		}
	}

	fclose(f);
	printf("loadParam(): read tokens from %s (%d/%d)\n", filename, token_cnt, rows * cols);
}

int main() {

	loadParam("../model/w1.param", (long double*) W1);
	loadParam("../model/w2.param", (long double*) W2);
	loadParam("../model/w3.param", (long double*) W3);
	loadParam("../model/w4.param", (long double*) W4);
	loadParam("../model/w5.param", (long double*) W5);

	loadParam("../model/b1.param", B1);
	loadParam("../model/b2.param", B2);
	loadParam("../model/b3.param", B3);
	loadParam("../model/b4.param", B4);
	loadParam("../model/b5.param", B5);
	
	

	return 0;
}
