#include "mnist_utils/mnist_backpropagation.h"

float E(float t[OUTPUT_LENGTH], float y[OUTPUT_LENGTH]) {
	float result = 0;
	for(int i = 0; i < OUPUT_LENGTH; i++) {
		float tmp = (y[i] - t[i]);
		result += (tmp * tmp);
	}
	return result / 2.0f;
}

void backOL(float t[OUTPUT_LENGTH], float iv[HL_LENGTH], float ov[OUTPUT_LENGTH], float newEI[OUTPUT_LENGTH]) {

	for(int j = 0; j < OUTPUT_LENGTH; j++) {
		newEI[j] = (ov[j] - t[j]) * ov[j] * (1 - ov[j]);
	}
			
	for(int i = 0; i < HL_LENGTH; i++) {
		for(int j = 0; j < OUTPUT_LENGTH; j++) {
			W5[i][j] -= (learning_rate * newEI[j] * iv[i]);
		}
	}

	for(int i = 0; i < OUTPUT_LEGNTH; i++) {
		B5[i] -= (learning_rate * newEI[i]);
	}
}

void backHL4(float prevEI[OUTPUT_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEI[HL_LENGTH]) {
	
	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < OUTPUT_LENGTH; j++) {
				tmp += (prevEI[j] * W5[i][j]);
			}
			newEI[i] = tmp;
		} else {
			newEI[i] = 0;
		}
	}

	for (int i = 0; i < HL_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W4[i][j] -= (learning_rate * newEI[j] * iv[i]);
		}
	}
	
	for(int i = 0; i < HL_LEGNTH; i++) {
		B4[i] -= (learning_rate * newEI[i]);
	}
}

void backHL3(float prevEI[HL_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEI[HL_LENGTH]) {
	
	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < HL_LENGTH; j++) {
				tmp += (prevEI[j] * W4[i][j]);
			}
			newEI[i] = tmp;
		} else {
			newEI[i] = 0;
		}
	}

	for (int i = 0; i < HL_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W3[i][j] -= (learning_rate * newEI[j] * iv[i]);
		}
	}

	for(int i = 0; i < HL_LEGNTH; i++) {
		B3[i] -= (learning_rate * newEI[i]);
	}

}

void backHL2(float prevEI[HL_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEI[HL_LENGTH]) {
	
	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < HL_LENGTH; j++) {
				tmp += (prevEI[j] * W3[i][j]);
			}
			newEI[i] = tmp;
		} else {
			newEI[i] = 0;
		}
	}

	for (int i = 0; i < HL_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W2[i][j] -= (learning_rate * newEI[j] * iv[i]);
		}
	}	
	for(int i = 0; i < HL_LEGNTH; i++) {
		B2[i] -= (learning_rate * newEI[i]);
	}

}

void backHL1(float prevEI[OUTPUT_LENGTH], float iv[INPUT_LENGTH] float ov[HL_LENGTH]) {
	
	float newEI[HL_LENGTH];

	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < HL_LENGTH; j++) {
				tmp += (prevEI[j] * W2[i][j]);
			}
			newEI[i] = tmp;
		} else {
			newEI[i] = 0;
		}
	}

	for (int i = 0; i < INPUT_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W1[i][j] -= (learning_rate * newEI[j] * iv[i]);
		}
	}

	for(int i = 0; i < HL_LEGNTH; i++) {
		B1[i] -= (learning_rate * newEI[i]);
	}

}

