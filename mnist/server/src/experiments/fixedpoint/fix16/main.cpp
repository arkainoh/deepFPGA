#include <stdio.h>
#include <time.h>
#include "feedforward.h"
#include "../../../mnist_utils/mnist_utils.h"
#include "../../../parameters/parameters.h"
#include "../../../../../../utils/fixedpoint/fixedpoint.h"

void getImage_fix16(FILE *imageFile, fix16 arr[], int iwl) {
	for(int i = 0; i < INPUT_LENGTH; i++) {
		float f = (((float) fgetc(imageFile)) / 255);
		arr[i] = float2fix(f, 16, iwl);
	}
}

int main() {
	fix16 pixels[INPUT_LENGTH];
	fix16 intervec1[HL_LENGTH];
	fix16 intervec2[HL_LENGTH];
	fix16 intervec3[HL_LENGTH];
	fix16 intervec4[HL_LENGTH];
	fix16 intervec5[OUTPUT_LENGTH];

	fix16 EH1[HL_LENGTH];
	fix16 EH2[HL_LENGTH];
	fix16 EH3[HL_LENGTH];
	fix16 EH4[HL_LENGTH];

	int correct;
	float gap;
	int pred;
	time_t startTime, endTime;
	int total_case;
	int correct_case;

	correct = 0; startTime = 0; endTime = 0; total_case = 0; correct_case = 0;

	FILE *testingimageFile, *testinglabelFile;
	testingimageFile = openMNISTImageFile("../../../../data/t10k-images-idx3-ubyte");
	testinglabelFile = openMNISTLabelFile("../../../../data/t10k-labels-idx1-ubyte");
	for(int imgCount = 0; imgCount < MNIST_MAX_TESTING_IMAGES; imgCount++) {

	total_case++;
	getImage_fix16(testingimageFile, pixels, IWL);
	MNIST_Label lbl = getLabel(testinglabelFile);

	startTime = clock();
	passIL(pixels, intervec1);
	passHL2(intervec1, intervec2);
	passHL3(intervec2, intervec3);
	passHL4(intervec3, intervec4);
	passOL(intervec4, intervec5);
	endTime = clock();
				
	fix16 maxVal = intervec5[0];
	pred = 0;

	for(int i = 1; i < 10; i++)	{
		if(maxVal < intervec5[i]) {
			maxVal = intervec5[i];
			pred = i;
		}
	}

	if(pred == lbl)	correct = 1;
	else correct = 0;

	if(correct){
		printf("[ correct ] ");
		correct_case++;
	} else {
		// showImg(img.pixel);
		printf("[incorrect] ");
	}
	printf("pred: %d, actual: %d / ", pred, lbl);

	gap = (float) (endTime - startTime) / CLOCKS_PER_SEC;
	printf("elapsed time: %dm %ds accuracy: %f\n", (int) gap / 60, (int) gap % 60, (float) correct_case / total_case);

	}


}
