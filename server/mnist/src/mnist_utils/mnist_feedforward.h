/*
 * mnist_feedforward.h
 *
 *  Created on: May 24, 2017
 *      Author: root
 */

#ifndef SRC_MNIST_UTILS_MNIST_FEEDFORWARD_H_
#define SRC_MNIST_UTILS_MNIST_FEEDFORWARD_H_
#include <stdio.h>
#include <string.h>
#include <time.h>

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

void passIL(float x[INPUT_LENGTH], float result[HL_LENGTH]);
void passHL2(float x[HL_LENGTH], float result[HL_LENGTH]);
void passHL3(float x[HL_LENGTH], float result[HL_LENGTH]);
void passHL4(float x[HL_LENGTH], float result[HL_LENGTH]);
void passOL(float x[HL_LENGTH], int *res);


#endif /* SRC_MNIST_UTILS_MNIST_FEEDFORWARD_H_ */
