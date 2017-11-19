#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../fixedpoint/fixedpoint.h"
#define TOKEN_LENGTH 24 // pos num: 24, neg num: 25
#define BUFFER_SIZE 1024

#define MODE_FLOAT32 0
#define MODE_FIX8 1
#define MODE_FIX16 2
#define MODE_EXP 3

// custom function overriding print_binary() from fixedpoint.h
void fprint_binary(FILE* f, int num, int len) {
	unsigned int mask = 1 << len - 1;
	
	fprintf(f, "0b");
	while(len--) {
		if(num & mask) fprintf(f, "1");
		else fprintf(f, "0");

		mask = mask >> 1;
	}
}

void copy_matrix(char* infilename, char* outfilename, int mode, int iwl) {
	char line[BUFFER_SIZE];
	FILE *fi;
	FILE *fo;
	
	static char token[TOKEN_LENGTH];
	static float* ptr = 0;
	int token_cursor = 0;
	int token_cnt = 0;
	int rows;
	int cols;
	
	fi = fopen(infilename, "r");
	fo = fopen(outfilename, "w");
	fgets(line, sizeof(line), fi);
	sscanf(line, "%d %d", &rows, &cols);
	
	fprintf(fo, "{{");
	while(fgets(line, sizeof(line), fi)) {
		int off = 0;

		// read token
		for(int i = 0; i < BUFFER_SIZE; i++) {
			if(off) break;
			switch(line[i]) {
			case ' ':
			case '\n':
			case '\0':
				if((token_cursor == TOKEN_LENGTH && token[0] != '-') ||
						(token_cursor == TOKEN_LENGTH + 1 && token[0] == '-')) {
					// save token
					token[token_cursor] = '\0';
					token_cursor = 0;
					token_cnt++;
					
					float f;
					sscanf(token, "%f", &f);

					switch(mode) {
					case MODE_FLOAT32:
						fprintf(fo, "%f", f);
					break;
					case MODE_FIX8:
						fix8 f8;
						f8 = fix(f, 8, iwl);
						fprint_binary(fo, f8, 8);
					break;
					case MODE_FIX16:
						fix16 f16;
						f16 = fix(f, 16, iwl);
						fprint_binary(fo, f16, 16);
					break;
					case MODE_EXP:
					default:
						fputs(token, fo);
					break;

					}

					if(token_cnt % cols == 0) {

						if(token_cnt == rows * cols) {
							fprintf(fo, "}};");
						} else {
							fprintf(fo, "}, {");
						}
						
					} else {
						fprintf(fo, ", ");
					}

				}

				if(line[i] == '\0') off = 1;
			break;

			default:
				token[token_cursor++] = line[i];
			break;
			}
		}
	}
	printf("Complete.\n# of elements: [%d / %d]\n", token_cnt, rows * cols);
	fclose(fi);
	fclose(fo);
}

int main(int argc, char* argv[]) {
	if(argc != 3 && argc != 4) {
		printf("usage: %s <input> <output> <option>\n\n", argv[0]);
		printf("arguments:\n");
		printf("<input>:  matrix file (*.mat)\n");
		printf("          e.g. test.mat\n");
		printf("          2 3\n");
		printf("          a11 a12 a13\n");
		printf("          a21 a22 a23\n");
		printf("<output>: name of file to which copied values are saved\n");
		printf("          values are organized like c-style array declaration\n");
		printf("          e.g. output.c\n");
		printf("          {{a11,a12,a13},{a21,a22,a23}};\n");
		printf("<option>: -e        use exponential notation (default)\n");
		printf("                    e.g. -1.234+e01\n");
		printf("          -float32  use basic float notation\n");
		printf("                    e.g. 3.141592\n");
		printf("          -fix8     use 8-bit fixed-point number\n");
		printf("                    e.g. 0b01010101\n");
		printf("          -fix16    use 16-bit fixed-point number\n");
		printf("                    e.g. 0b0101010101010101\n");
		exit(1);
	}

	int mode = MODE_EXP;
	int iwl = 3;

	if(argc == 4) {
		if(!strcmp(argv[3], "-float32")) {
			mode = MODE_FLOAT32;
		} else if(!strcmp(argv[3], "-fix8")) {
			mode = MODE_FIX8;
			printf("# of bits allocated for integer (except sign): ");
			scanf("%d", &iwl);
		} else if(!strcmp(argv[3], "-fix16")) {
			mode = MODE_FIX16;
			printf("# of bits allocated for integer (except sign): ");
			scanf("%d", &iwl);
		}
	}

	copy_matrix(argv[1], argv[2], mode, iwl);

	return 0;
}
