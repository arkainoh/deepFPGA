#include <stdio.h>
#define INPUT_LENGTH 784
#define HL_SIZE 512
#define OUTPUT_LENGTH 10

double X[INPUT_LENGTH + 1];
double W1[INPUT_LENGTH + 1][HL_SIZE];
double W2[HL_SIZE + 1][HL_SIZE];
double W3[HL_SIZE + 1][HL_SIZE];
double W4[HL_SIZE + 1][HL_SIZE];
double W5[HL_SIZE + 1][OUTPUT_LENGTH];


