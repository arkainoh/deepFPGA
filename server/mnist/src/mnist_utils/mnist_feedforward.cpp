/*
 * mnist_feedforward.cpp


 *
 *  Created on: May 24, 2017
 *      Author: root
 */

#include "mnist_utils/mnist_feedforward.h"

void passIL(float x[INPUT_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B1[n]; // copy b to result

	for(int m = 0; m < INPUT_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += (x[m] * W1[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
	{
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}

void passHL2(float x[HL_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B2[n]; // copy b to result

	for(int m = 0; m < HL_LENGTH; m++)
		// row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += (x[m] * W2[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
	{
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}
void passHL3(float x[HL_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B3[n]; // copy b to result

	for(int m = 0; m < HL_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += (x[m] * W3[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
	{
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}
void passHL4(float x[HL_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B4[n]; // copy b to result

	for(int m = 0; m < HL_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += (x[m] * W4[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
	{
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}


void passOL(float x[HL_LENGTH], int *res) {

//
	float result[OUTPUT_LENGTH];

	for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] = B5[n]; // copy b to result

	for(int m = 0; m < HL_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] += (x[m] * W5[m][n]);

	float max = result[0];
	int argmax = 0;
	for(int n = 1; n < OUTPUT_LENGTH; n++) // get max value (softmax part)
		if(result[n] > max) {
			max = result[n];
			argmax = n;
		}

	*res = argmax;

}
