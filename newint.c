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



int loadW() {

	char line[BUFFER_SIZE];
	FILE *f;
	f = fopen("test.w", "r");
	
	char token[TOKEN_LENGTH];
	int token_cursor = 0;

	int row = 0;
	int col = 0;

	while(fgets(line, sizeof(line), f)) {
		puts(line);
		printf("read length: %d\n", strlen(line));
		int on = 1;

		//read token
		for(int i = 0; i < BUFFER_SIZE; i++) {
			if (on) {
				switch(line[i]) {
				case ' ':
					// save token
					
					token_cursor = 0; // reset cursor
				break;

				case '\n':
					if (token_cursor == 24 || token_cursor == 25) {
						
					}

				break;

				case '\0':
					// save token
					on = 0;
				break;

				default:
					token[token_cusor++] = line[i];
				break;
				}
			}
		}

		puts("===============================+");
	}

}

int main() {

	loadW();
	return 0;
}
