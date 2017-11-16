#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <random>
#include "mnist_util.h"


uint32_t flipBytes(uint32_t n){

    uint32_t b0, b1, b2, b3;

    b0 = (n & 0x000000ff) << 24u;
    b1 = (n & 0x0000ff00) <<  8u;
    b2 = (n & 0x00ff0000) >>  8u;
    b3 = (n & 0xff000000) >> 24u;

    return (b0 | b1 | b2 | b3);
}

/**
 * @details Read MNIST image file header
 * @see http://yann.lecun.com/exdb/mnist/ for definition details
 */

void readImageFileHeader(FILE *imageFile, MNIST_ImageFileHeader *ifh) {

    ifh->magicNumber = 0;
    ifh->maxImages   = 0;
    ifh->imgWidth    = 0;
    ifh->imgHeight   = 0;

    fread(&ifh->magicNumber, 4, 1, imageFile);
    ifh->magicNumber = flipBytes(ifh->magicNumber);

    fread(&ifh->maxImages, 4, 1, imageFile);
    ifh->maxImages = flipBytes(ifh->maxImages);

    fread(&ifh->imgWidth, 4, 1, imageFile);
    ifh->imgWidth = flipBytes(ifh->imgWidth);

    fread(&ifh->imgHeight, 4, 1, imageFile);
    ifh->imgHeight = flipBytes(ifh->imgHeight);
}

/**
 * @details Read MNIST label file header
 * @see http://yann.lecun.com/exdb/mnist/ for definition details
 */

void readLabelFileHeader(FILE *imageFile, MNIST_LabelFileHeader *lfh) {

    lfh->magicNumber = 0;
    lfh->maxImages   = 0;

    fread(&lfh->magicNumber, 4, 1, imageFile);
    lfh->magicNumber = flipBytes(lfh->magicNumber);

    fread(&lfh->maxImages, 4, 1, imageFile);
    lfh->maxImages = flipBytes(lfh->maxImages);

}

/**
 * @details Open MNIST image file and read header info
 * by reading the header info, the read pointer
 * is moved to the position of the 1st IMAGE
 */

FILE *openMNISTImageFile(std::string fileName) {

    FILE *imageFile;
    imageFile = fopen (fileName.c_str(), "rb");
    if (imageFile == NULL) {
        printf("file not found: %s\n", fileName.c_str());
        exit(0);
    }

    MNIST_ImageFileHeader imageFileHeader;
    readImageFileHeader(imageFile, &imageFileHeader);

    return imageFile;
}

/**
 * @details Open MNIST label file and read header info
 * by reading the header info, the read pointer
 * is moved to the position of the 1st LABEL
 */

FILE *openMNISTLabelFile(std::string fileName){

    FILE *labelFile;
    labelFile = fopen (fileName.c_str(), "rb");
    if (labelFile == NULL) {
        printf("file not found: %s\n", fileName.c_str());
        exit(0);
    }

    MNIST_LabelFileHeader labelFileHeader;
    readLabelFileHeader(labelFile, &labelFileHeader);

    return labelFile;
}

/**
 * @details Returns the next image in the given MNIST image file
 */

MNIST_Image getImage_net(char arr[]) {
    MNIST_Image img;

    //result = fread(&img, sizeof(img), 1, imageFile);
    for(int i = 1 ; i <INPUT_LENGTH + 1;i++) {
    	img.pixel[i] = (float) arr[i];
    }

    return img;
}

MNIST_Label getLabel_net(char arr[]) {

    MNIST_Label lbl;
    size_t result;
    result = arr[0];
    if (result != 1) {
        printf("\nerror occurred reading LABEL file\n");
        exit(1);
    }

    return lbl;
}

MNIST_Image getImage(FILE *imageFile) {

    MNIST_Image img;

    //result = fread(&img, sizeof(img), 1, imageFile);
    for(int i =0 ; i <INPUT_LENGTH;i++) {
    	img.pixel[i] = (float) ((float) fgetc(imageFile)) / 255;
    }

    return img;
}

/**
 * @details Returns the next label in the given MNIST label file
 */

MNIST_Label getLabel(FILE *labelFile) {

    MNIST_Label lbl;
    size_t result;
    result = fread(&lbl, sizeof(lbl), 1, labelFile);
    if (result!=1) {
        printf("\nError when reading LABEL file! Abort!\n");
        exit(1);
    }

    return lbl;
}

void loadParam(char* filename, float* mat) {

	char line[BUFFER_SIZE];
	FILE *f;

	static char token[TOKEN_LENGTH];

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

//
//
//int loadImg(char* filename, float x[INPUT_LENGTH]) {
//	char line[BUFFER_SIZE];
//	FILE* f;
//	int token_cnt = 0;
//
//	f = fopen(filename, "r");
//	while (fgets(line, sizeof(line), f)) {
//		char* token;
//		int val;
//
//		token = strtok(line, " ");
//
//		while(token != NULL) {
//			sscanf(token, "%x", &val);
//			x[token_cnt++] = (float) val;
//			token = strtok(NULL, " ");
//		}
//	}
//
//	fclose(f);
//	return token_cnt;
//}

void showImg(float x[INPUT_LENGTH]) {

	for(int i = 0; i < INPUT_LENGTH; i++) {
		if(x[i] > 0) printf("X ");
		else printf("  ");

		if(i % 28 == 0) {
			puts("");
		}
	}

	puts("");
}

float evalToken(char* token) {
	float val;
	sscanf(token, "%f", &val);
	return val;
}
