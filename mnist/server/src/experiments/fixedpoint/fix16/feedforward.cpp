#include "feedforward.h"

void passIL(fix16 x[INPUT_LENGTH], fix16 result[HL_LENGTH]) {

	fix16 _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B1[n]; // copy b to result

	for(int m = 0; m < INPUT_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += mul_fix(x[m], (fix16)W1[m][n], 16, IWL);

	for(int n = 0; n < HL_LENGTH; n++) // reLU
	{
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}
	
}

void passHL2(fix16 x[HL_LENGTH], fix16 result[HL_LENGTH]) {

	fix16 _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B2[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += mul_fix(x[m], (fix16)W2[m][n], 16, IWL);
	
	for(int n = 0; n < HL_LENGTH; n++) {
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}
	
}

void passHL3(fix16 x[HL_LENGTH], fix16 result[HL_LENGTH]) {

	fix16 _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B3[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += mul_fix(x[m], (fix16)W3[m][n], 16, IWL);

	for(int n = 0; n < HL_LENGTH; n++) {
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}
	
}
void passHL4(fix16 x[HL_LENGTH], fix16 result[HL_LENGTH]) {

	fix16 _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B4[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += mul_fix(x[m], (fix16)W4[m][n], 16, IWL);

	for(int n = 0; n < HL_LENGTH; n++) {
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}

void passOL(fix16 x[HL_LENGTH], fix16 result[OUTPUT_LENGTH]) {
	
	fix16 _result[OUTPUT_LENGTH];

	for(int n = 0; n < OUTPUT_LENGTH; n++) _result[n] = B5[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < OUTPUT_LENGTH; n++) _result[n] += mul_fix(x[m], (fix16)W5[m][n], 16, IWL);

	// no softmax
	for(int n = 0; n < OUTPUT_LENGTH; n++)
		result[n] = _result[n];
}

