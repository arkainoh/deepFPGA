#include "mnist_utils/mnist_backpropagation2.h"

float E(float t[OUTPUT_LENGTH], float y[OUTPUT_LENGTH]) {
	for(int i = 0; i < OUPUT_LENGTH; i++) {
		float tmp = (y[i] - t[i]);
		result += (tmp * tmp);
	}
	return result / 2.0f;
}

void getError(float y[OUTPUT_LENGTH], float t[OUTPUT_LENGTH], float e[OUTPUT_LENGTH]) {
	for(int i = 0; i < OUTPUT_LENGTH; i++) {
		e[i] = y[i] - t[i];
	}
}

void backOL(float e[OUTPUT_LENGTH], float iv[HL_LENGTH], float ov[OUTPUT_LENGTH]) {

	for(int i = 0; i < HL_LENGTH; i++) {
		for(int j = 0; j < OUTPUT_LENGTH; j++) {
			W5[i][j] -= (learning_rate * e[j] * iv[i]);
		}
	}

	for(int i = 0; i < OUTPUT_LEGNTH; i++) {
		B5[i] -= (learning_rate * e[j]);
	}
}

void backHL4(float e[OUTPUT_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < OUTPUT_LENGTH; j++) {
				tmp += (e[j] * W5[i][j]);
			}
			newEH[i] = tmp;
		} else {
			newEH[i] = 0;
		}
	}

	for (int i = 0; i < HL_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W4[i][j] -= (learning_rate * newEH[j] * iv[i]);
		}
	}
	
	for(int i = 0; i < HL_LEGNTH; i++) {
		B4[i] -= (learning_rate * newEH[i]);
	}
}

void backHL3(float prevEH[HL_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < HL_LENGTH; j++) {
				tmp += (prevEH[j] * W4[i][j]);
			}
			newEH[i] = tmp;
		} else {
			newEH[i] = 0;
		}
	}

	for (int i = 0; i < HL_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W3[i][j] -= (learning_rate * newEH[j] * iv[i]);
		}
	}

	for(int i = 0; i < HL_LEGNTH; i++) {
		B3[i] -= (learning_rate * newEH[i]);
	}

}

void backHL2(float prevEH[HL_LENGTH], float iv[HL_LENGTH] float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < HL_LENGTH; j++) {
				tmp += (prevEH[j] * W3[i][j]);
			}
			newEH[i] = tmp;
		} else {
			newEH[i] = 0;
		}
	}

	for (int i = 0; i < HL_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W2[i][j] -= (learning_rate * newEH[j] * iv[i]);
		}
	}	
	for(int i = 0; i < HL_LEGNTH; i++) {
		B2[i] -= (learning_rate * newEH[i]);
	}

}

void backHL1(float prevEH[OUTPUT_LENGTH], float iv[INPUT_LENGTH] float ov[HL_LENGTH]) {
	
	float newEH[HL_LENGTH];

	for (int i = 0; i < HL_LENGTH; i++) {
		float tmp = 0;
		if(ov[i]) {
			for (int j = 0; j < HL_LENGTH; j++) {
				tmp += (prevEH[j] * W2[i][j]);
			}
			newEH[i] = tmp;
		} else {
			newEH[i] = 0;
		}
	}

	for (int i = 0; i < INPUT_LENGTH; i++) {
		for (int j = 0; j < HL_LENGTH; j++) {
			W1[i][j] -= (learning_rate * newEH[j] * iv[i]);
		}
	}

	for(int i = 0; i < HL_LEGNTH; i++) {
		B1[i] -= (learning_rate * newEH[i]);
	}

}

