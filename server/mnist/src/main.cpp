/*
 * main.cpp
 *
 *  Created on: May 21, 2017
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "mnist_utils/mnist-utils.h"
#include "mnist_utils/mnist_feedforward.h"

int main(int argc, char *argv[]) {

	while (true)
	{
		printf("please, enter the option, \n1: testing with /test/t10kblahblah data\n	\
				2: training visualization\
				3: accuracy control\n\
				4: exit\n");
		int where;
		std::cin >> where;
		switch(where){

		case 1:
		{
			int total_case=0;
			int correct_case=0;
			FILE *imageFile, *labelFile;
			imageFile = openMNISTImageFile("../test/t10k-images-idx3-ubyte");
			labelFile = openMNISTLabelFile("../test/t10k-labels-idx1-ubyte");

			float intervec1[HL_LENGTH];
			float intervec2[HL_LENGTH];
			float intervec3[HL_LENGTH];
			float intervec4[HL_LENGTH];

			for (int imgCount=0; imgCount<MNIST_MAX_TRAINING_IMAGES; imgCount++){
				total_case++;
				int correct = 0;
				float gap;
				int pred;
				time_t startTime = 0, endTime = 0;

				MNIST_Image img = getImage(imageFile);
				MNIST_Label lbl = getLabel(labelFile);

				startTime = clock();
				passIL(img.pixel, intervec1);
				passHL2(intervec1, intervec2);
				passHL3(intervec2, intervec3);
				passHL4(intervec3, intervec4);
				passOL(intervec4, &pred);
				endTime = clock();
				printf("prediction: %d actual: %d/ ", pred,lbl);
				if(pred == lbl)
					correct = 1;

				if(correct){
					printf("correct\t/ ");
					correct_case++;
				}
				else
					printf("not\t/ ");

				gap = (float)(endTime - startTime)/CLOCKS_PER_SEC;
				printf("elapsed time: %f precision: %f\n", gap,(float)correct_case/total_case);
			}
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			break;
		}
		case 4:
		{
			break;
		}
		default:
		{
			break;
		}
		}

	}


	return 0;
}
