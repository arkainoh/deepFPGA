#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "weights/defines.h"


typedef struct MNIST_ImageFileHeader MNIST_ImageFileHeader;
typedef struct MNIST_LabelFileHeader MNIST_LabelFileHeader;


typedef struct MNIST_Image MNIST_Image;
typedef uint8_t MNIST_Label;

struct MNIST_Image{
    float pixel[MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT];
};




/**
 * @brief Data block defining a MNIST image file header
 * @attention The fields in this structure are not used.
 * What matters is their byte size to move the file pointer
 * to the first image.
 * @see http://yann.lecun.com/exdb/mnist/ for details
 */

struct MNIST_ImageFileHeader{
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

struct MNIST_LabelFileHeader{
    uint32_t magicNumber;
    uint32_t maxImages;
};

void loadParam(char* filename, float* mat);
//int loadImg(char* filename, float x[INPUT_LENGTH]);
void showImg(float x[INPUT_LENGTH]);
float evalToken(char* token);
FILE *openMNISTImageFile(std::string fileName);
FILE *openMNISTLabelFile(std::string fileName);
MNIST_Image getImage(FILE *imageFile);
MNIST_Label getLabel(FILE *labelFile);
MNIST_Label getLabel_net(char arr[]);
MNIST_Image getImage_net(char arr[]);
