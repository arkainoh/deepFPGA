#include <stdio.h>
#include <string.h>
#define INPUT_LENGTH 784
#define HL_SIZE 512
#define OUTPUT_LENGTH 10
#define TOKEN_LENGTH 25 // pos num: 24, neg num: 25
#define BUFFER_SIZE 1024

double X[INPUT_LENGTH + 1];
double W1[INPUT_LENGTH + 1][HL_SIZE];
double W2[HL_SIZE + 1][HL_SIZE];
double W3[HL_SIZE + 1][HL_SIZE];
double W4[HL_SIZE + 1][HL_SIZE];
double W5[HL_SIZE + 1][OUTPUT_LENGTH];


int saveToken() {
	
	return 0;
}

void loadW() {
	puts("loadW()");
	char line[BUFFER_SIZE];
	FILE *f;
	f = fopen("test.w", "r");
	
	char static token[TOKEN_LENGTH];
	int token_cursor = 0;

	//int row = 0;
	//int col = 0;

	fgets(line, sizeof(line), f);
	printf("First row: %s\n", line);

	while(fgets(line, sizeof(line), f)) {
		puts("reading line...");
		puts(line);
		printf("read length: %d\n", strlen(line));
		puts("");
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
			break;

			case '\n':
			case '\0':
				if ((token_cursor == 24 && token[0] != '-') || (token_cursor == 25 && token[0] == '-')) {
					// save token
					token[token_cursor] = '\0';
					token_cursor = 0;
					puts(token);
				}

				if (line[i] == '\0') {
					off = 1;
					puts("end of the line");
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

	loadW();
	return 0;
}
