#ifndef SRC_MNIST_UTILS_MNIST_NN_H_
#define SRC_MNIST_UTILS_MNIST_NN_H_
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../weights/W1.h"
#include "../weights/W2.h"
#include "../weights/W3.h"
#include "../weights/W4.h"
#include "../weights/W5.h"

#include "../weights/B1.h"
#include "../weights/B2.h"
#include "../weights/B3.h"
#include "../weights/B4.h"
#include "../weights/B5.h"

#include "../weights/defines.h"

void getError(float y[OUTPUT_LENGTH], int label, float e[OUTPUT_LENGTH]);
float getLoss(float y[OUTPUT_LENGTH], int label);
void backOL(float e[OUTPUT_LENGTH], float iv[HL_LENGTH], float ov[OUTPUT_LENGTH]);
void backHL4(float e[OUTPUT_LENGTH], float iv[HL_LENGTH], float ov[HL_LENGTH], float newEH[HL_LENGTH]);
void backHL3(float prevEI[HL_LENGTH], float iv[HL_LENGTH], float ov[HL_LENGTH], float newEH[HL_LENGTH]);
void backHL2(float prevEI[HL_LENGTH], float iv[HL_LENGTH], float ov[HL_LENGTH], float newEH[HL_LENGTH]);
void backHL1(float prevEI[HL_LENGTH], float iv[INPUT_LENGTH], float ov[HL_LENGTH]);

void passIL(float x[INPUT_LENGTH], float result[HL_LENGTH]);
void passHL2(float x[HL_LENGTH], float result[HL_LENGTH]);
void passHL3(float x[HL_LENGTH], float result[HL_LENGTH]);
void passHL4(float x[HL_LENGTH], float result[HL_LENGTH]);
void passOL(float x[HL_LENGTH],  float result[OUTPUT_LENGTH]);
void weightInitialization(void);
void setLearningRate(float val);

#endif
