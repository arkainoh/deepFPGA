#include "mnist_nn.h"
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>
#include <stdlib.h>

float learning_rate = 0.0001;

void setLearningRate(float val) {
	learning_rate = val;
}

float getLearningRate() {
	return learning_rate;
}

void xavierInitialization(float* mat, int fan_in, int fan_out) {
	std::random_device rd;
	std::mt19937 gen(rd());
	float range = sqrt(6.0 / (fan_in + fan_out));
	std::uniform_real_distribution<float> xavier(-range, range);
	srand(time(NULL));
	for(int i = 0; i < fan_in * fan_out; i++) {
		*(mat + i) = xavier(gen);		
	}
}

void normalDistribution(float *mat, int fan_in, int fan_out) {
	std::default_random_engine gen;
	std::normal_distribution<float> nd(0.0, 1.0);
	srand(time(NULL));
	for(int i = 0; i < fan_in * fan_out; i++) {
		*(mat + i) = nd(gen);		
	}
}

void uniformDistribution(float *mat, int fan_in, int fan_out) {
	std::random_device rd;
	std::mt19937 gen(rd());
	float range = 0.1;
	std::uniform_real_distribution<float> ud(-range, range);
	srand(time(NULL));
	for(int i = 0; i < fan_in * fan_out; i++) {
		*(mat + i) = ud(gen);		
	}
}

void constantInitialization(float *mat, int fan_in, int fan_out, float val) {
	for(int i = 0; i < fan_in * fan_out; i++) {
		*(mat + i) = val;
	}
}

void parameterInitialization_xavier(void) {
	
	xavierInitialization((float*) W1, INPUT_LENGTH, HL_LENGTH);
	xavierInitialization((float*) W2, HL_LENGTH, HL_LENGTH);
	xavierInitialization((float*) W3, HL_LENGTH, HL_LENGTH);
	xavierInitialization((float*) W4, HL_LENGTH, HL_LENGTH);
	xavierInitialization((float*) W5, HL_LENGTH, OUTPUT_LENGTH);
	
	constantInitialization(B1, 1, HL_LENGTH, 0);
	constantInitialization(B2, 1, HL_LENGTH, 0);
	constantInitialization(B3, 1, HL_LENGTH, 0);
	constantInitialization(B4, 1, HL_LENGTH, 0);
	constantInitialization(B5, 1, OUTPUT_LENGTH, 0);

}

void parameterInitialization_uniform(void) {
	uniformDistribution((float*) W1, INPUT_LENGTH, HL_LENGTH);
	uniformDistribution((float*) W2, HL_LENGTH, HL_LENGTH);
	uniformDistribution((float*) W3, HL_LENGTH, HL_LENGTH);
	uniformDistribution((float*) W4, HL_LENGTH, HL_LENGTH);
	uniformDistribution((float*) W5, HL_LENGTH, OUTPUT_LENGTH);
	
	constantInitialization(B1, 1, HL_LENGTH, 0);
	constantInitialization(B2, 1, HL_LENGTH, 0);
	constantInitialization(B3, 1, HL_LENGTH, 0);
	constantInitialization(B4, 1, HL_LENGTH, 0);
	constantInitialization(B5, 1, OUTPUT_LENGTH, 0);
	
}

void passIL(float x[INPUT_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B1[n]; // copy b to result
	
	for(int m = 0; m < INPUT_LENGTH; m++) // row-wise matmul
		for(int n = 0; n < HL_LENGTH; n++) {
			_result[n] += (x[m] * W1[m][n]);
		}

	for(int n = 0; n < HL_LENGTH; n++) // reLU
	{
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}
}

void passHL2(float x[HL_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B2[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += (x[m] * W2[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) {
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}

void passHL3(float x[HL_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B3[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += (x[m] * W3[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) {
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}

void passHL4(float x[HL_LENGTH], float result[HL_LENGTH]) {

	float _result[HL_LENGTH];

	for(int n = 0; n < HL_LENGTH; n++) _result[n] = B4[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < HL_LENGTH; n++) _result[n] += (x[m] * W4[m][n]);

	for(int n = 0; n < HL_LENGTH; n++) {
		if(_result[n] < 0) result[n] = 0;
		else result[n] = _result[n];
	}

}

void passOL(float x[HL_LENGTH], float result[OUTPUT_LENGTH]) {

	for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] = B5[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < OUTPUT_LENGTH; n++) result[n] += (x[m] * W5[m][n]);

}

void softmax(float x[OUTPUT_LENGTH], float result[OUTPUT_LENGTH]) {
	
	float _result[OUTPUT_LENGTH];
	float maxValue = x[0];
	for(int n = 1; n < OUTPUT_LENGTH; n++) {
		if(maxValue < x[n]) {
			maxValue = x[n];
		}
	}
	
	float sum = 0;
	for(int n = 0; n < OUTPUT_LENGTH; n++) {
		_result[n] = exp(x[n] - maxValue);	
		sum += _result[n];
	}

	for(int n = 0; n < OUTPUT_LENGTH; n++) {
		result[n] = _result[n] / sum ;
	}

}

void getError(float y[OUTPUT_LENGTH], int label, float e[OUTPUT_LENGTH]) {
	for(int i = 0; i < OUTPUT_LENGTH; i++) {
		e[i] = y[i];
		if (i == label) {
			e[i] -= 1;
		}
	}
}

float getLoss(float y[OUTPUT_LENGTH], int label) {
	return -log(y[label]);
}

void backOL(float e[OUTPUT_LENGTH], float iv[HL_LENGTH], float ov[OUTPUT_LENGTH]) {

	for(int i = 0; i < HL_LENGTH; i++) {
		for(int j = 0; j < OUTPUT_LENGTH; j++) {
			W5[i][j] -= (learning_rate * e[j] * iv[i]);
		}
	}

	for(int i = 0; i < OUTPUT_LENGTH; i++) {
		B5[i] -= (learning_rate * e[i]);
	}
}

void backHL4(float e[OUTPUT_LENGTH], float iv[HL_LENGTH], float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
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
	
	for(int i = 0; i < HL_LENGTH; i++) {
		B4[i] -= (learning_rate * newEH[i]);
	}
}

void backHL3(float prevEH[HL_LENGTH], float iv[HL_LENGTH], float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
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

	for(int i = 0; i < HL_LENGTH; i++) {
		B3[i] -= (learning_rate * newEH[i]);
	}

}

void backHL2(float prevEH[HL_LENGTH], float iv[HL_LENGTH], float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
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

	for(int i = 0; i < HL_LENGTH; i++) {
		B2[i] -= (learning_rate * newEH[i]);
	}

}

void backHL1(float prevEH[HL_LENGTH], float iv[INPUT_LENGTH] ,float ov[HL_LENGTH]) {
	
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

	for(int i = 0; i < HL_LENGTH; i++) {
		B1[i] -= (learning_rate * newEH[i]);
	}

}
