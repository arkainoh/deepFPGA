#include "mnist_nn.h"
#include <math.h>
#include <random>
#include <iostream>
#include <time.h>
#include <stdlib.h>

void weightInitialization(void) {
	int A = 4;
	std::random_device rd;
	std::mt19937 gen(rd());

	//distribution for weights
	std::uniform_real_distribution<float> disgogo(-0.1,0.1);
	std::uniform_real_distribution<float> dis(-sqrt(3/(784*512)),sqrt(3/(784*512)) );
	std::uniform_real_distribution<float> dis1(-sqrt(3/(512*512)),sqrt(3/(512*512)) );	
	std::uniform_real_distribution<float> dis2(-sqrt(3/(10*512)),sqrt(3/(10*512)) );

	//distribution for bias
	std::uniform_real_distribution<float> dis3(-sqrt(3/(512)),sqrt(3/(512)) );
	std::uniform_real_distribution<float> dis4(-sqrt(3/(10)),sqrt(3/(10)) );

	srand(time(NULL));

	for(int i=0;i<784;i++) {
		for(int j=0;j<512;j++) {
			 W1[i][j] = disgogo(gen);	
			 //W1[i][j] = dis(gen);	
			 //W1[i][j] = rand()%A;
		}
	}
	
	for(int i=0;i<512;i++) {
		for(int j=0;j<512;j++) {	
			 //W2[i][j] = dis1(gen);
			 //W3[i][j] = dis1(gen);
			 //W4[i][j] = dis1(gen);
	
			 W2[i][j] = disgogo(gen);
			 W3[i][j] = disgogo(gen);
			 W4[i][j] = disgogo(gen);
	
			 //W2[i][j] = rand()%A;
			 //W3[i][j] = rand()%A;
			 //W4[i][j] = rand()%A;
		
		}
	}
	for(int i =0;i<512;i++) {
		for(int j=0;j<10;j++)	{
			W5[i][j] = disgogo(gen);
			//W5[i][j] = rand()%A;
		}
	}
	
	// bias
	for(int i=0;i<512;i++) {
		B1[i] = disgogo(gen);
		B2[i] = disgogo(gen);
		B3[i] = disgogo(gen);
		B4[i] = disgogo(gen);
		
		//B1[i] = rand()%A;
		//B2[i] = rand()%A;
		//B3[i] = rand()%A;
		//B4[i] = rand()%A;
	
	}
	for(int i=0;i<10;i++)	{
		B5[i] = disgogo(gen);
	}
}


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

	float _result[OUTPUT_LENGTH];
	for(int n = 0; n < OUTPUT_LENGTH; n++) _result[n] = B5[n];

	for(int m = 0; m < HL_LENGTH; m++)
		for(int n = 0; n < OUTPUT_LENGTH; n++) _result[n] += (x[m] * W5[m][n]);

	// softmax
	float maxValue = _result[0];
	for(int n = 1; n < OUTPUT_LENGTH; n++) {
		if(maxValue < _result[n]) {
			maxValue = _result[n];
		}
	}
	
	float sum = 0;
	for(int n = 0; n < OUTPUT_LENGTH; n++) {
			sum += exp(_result[n] - maxValue);
	}

	for(int n = 0; n < OUTPUT_LENGTH; n++) {
		result[n] = exp(_result[n] - maxValue) / sum ;
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

void backHL3(float prevEH[HL_LENGTH], float iv[HL_LENGTH],float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
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

void backHL2(float prevEH[HL_LENGTH], float iv[HL_LENGTH],float ov[HL_LENGTH], float newEH[HL_LENGTH]) {
	
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
