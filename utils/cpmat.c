#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TOKEN_LENGTH 24 // pos num: 24, neg num: 25
#define BUFFER_SIZE 1024

void copy_matrix(char* infilename, char* outfilename) {
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
					fputs(token, fo);
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
	if(argc != 3) {
		printf("usage: %s <input> <output>\n\n", argv[0]);
		printf("arguments:\n");
		printf("<input>: matrix file (*.mat)\n");
		printf("         e.g. test.mat\n");
		printf("         2 3\n");
		printf("         a11 a12 a13\n");
		printf("         a21 a22 a23\n");
		printf("<output>: name of file to which copied values are saved\n");
		printf("          values are organized like c-style array declaration\n");
		printf("          e.g. output.c\n");
		printf("          {{a11,a12,a13},{a21,a22,a23}};\n");
		exit(1);
	}

	copy_matrix(argv[1], argv[2]);

	return 0;
}
