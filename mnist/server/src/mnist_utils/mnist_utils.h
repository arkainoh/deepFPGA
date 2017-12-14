#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <random>
#include "../parameters/parameters.h"

#define TRAIN_IMAGES "train-images-idx3-ubyte"
#define TRAIN_LABELS "train-labels-idx1-ubyte"
#define TEST_IMAGES "t10k-images-idx3-ubyte"
#define TEST_LABELS "t10k-labels-idx1-ubyte"

#define N_TRAIN_IMAGES 60000 // number of images + labels in the TRAIN file/s
#define N_TEST_IMAGES 10000 // number of images + labels in the TEST file/s
#define IMAGE_WIDTH 28 // image width in pixel
#define IMAGE_HEIGHT 28 // image height in pixel

typedef struct MNIST_ImageFileHeader MNIST_ImageFileHeader;
typedef struct MNIST_LabelFileHeader MNIST_LabelFileHeader;

typedef struct MNIST_Image MNIST_Image;
typedef uint8_t MNIST_Label;

struct MNIST_Image {
    float pixel[IMAGE_WIDTH * IMAGE_HEIGHT];
};

/**
 * @brief Data block defining a MNIST image file header
 * @attention The fields in this structure are not used.
 * What matters is their byte size to move the file pointer
 * to the first image.
 * @see http://yann.lecun.com/exdb/mnist/ for details
 */

struct MNIST_ImageFileHeader {
    uint32_t magicNumber;
    uint32_t maxImages;
    uint32_t imgWidth;
    uint32_t imgHeight;
};

/**
 * @brief Data block defining a MNIST label file header
 * @attention The fields in this structure are not used.
 * What matters is their byte size to move the file pointer
 * to the first label.
 * @see http://yann.lecun.com/exdb/mnist/ for details
 */

struct MNIST_LabelFileHeader {
    uint32_t magicNumber;
    uint32_t maxImages;
};

void loadParam(char* filename, float* mat);
//int loadImg(char* filename, float x[INPUT_LENGTH]);
void showImage(MNIST_Image* img);
float evalToken(char* token);
FILE *openMNISTImageFile(std::string fileName);
FILE *openMNISTLabelFile(std::string fileName);
MNIST_Image getImage(FILE *imageFile);
MNIST_Label getLabel(FILE *labelFile);
MNIST_Image getImage(float arr[]);
MNIST_Image getImage(FILE *imageFile, int index);
MNIST_Label getLabel(FILE *imageFile, int index);
