#ifndef SRC_MNIST_UTILS_MNIST_BACKPROPAGATION_H_
#define SRC_MNIST_UTILS_MNIST_BACKPROPAGATION_H_
#include <stdio.h>
#include <string.h>

#include "weights/W1.h"
#include "weights/W2.h"
#include "weights/W3.h"
#include "weights/W4.h"
#include "weights/W5.h"

#include "weights/B1.h"
#include "weights/B2.h"
#include "weights/B3.h"
#include "weights/B4.h"
#include "weights/B5.h"

#include "weights/defines.h"

float E(float t[OUTPUT_LENGTH], float y[OUTPUT_LENGTH]);
void backOL(float t[OUTPUT_LENGTH], float iv[HL_LENGTH], float ov[OUTPUT_LENGTH], float newEI[OUTPUT_LENGTH]);
void backHL4(float prevEI[OUTPUT_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEI[HL_LENGTH]);
void backHL3(float prevEI[HL_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEI[HL_LENGTH]);
void backHL2(float prevEI[HL_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEI[HL_LENGTH]);
void backHL1(float prevEI[OUTPUT_LENGTH], float iv[INPUT_LENGTH] float ov[HL_LENGTH]);

#endif
