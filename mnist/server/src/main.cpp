#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#include "mnist_utils/mnist_utils.h"
#include "mnist_utils/mnist_nn.h"
#include "network/networking.h"
#include "parameters/parameters.h"

#define DATA_PATH "../data/"

#define TRAINING_EPOCH 20
#define TRAINING_BATCH 10000

void shuffle(int* arr, int N) {
	srand(time(NULL));
	for(int i = N - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}
}

int main(int argc, char *argv[]) {

	float intervec1[HL_LENGTH];
	float intervec2[HL_LENGTH];
	float intervec3[HL_LENGTH];
	float intervec4[HL_LENGTH];
	float intervec5[OUTPUT_LENGTH];
	float est[OUTPUT_LENGTH];

	float EH1[HL_LENGTH];
	float EH2[HL_LENGTH];
	float EH3[HL_LENGTH];
	float EH4[HL_LENGTH];
	float e[OUTPUT_LENGTH];
	float t[OUTPUT_LENGTH];	

	int correct;
	float gap, total_time, avg_time;
	int pred;
	time_t startTime, endTime;
	int total_case;
	int correct_case;

	while(true)
	{
		printf("\r\n\
				<MENU>\r\n\
				1: connect to client\n\r\
				2: learn\n\r\
				3: test\n\r\
				4: exit\n\r");

		int menu;
		std::cin >> menu;
		switch(menu){

			case 1:
			{
				correct = 0; startTime = 0; endTime = 0; total_case = 0; correct_case = 0;
				char client_message[2000];
				struct sockaddr_in *server = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
				struct sockaddr_in *client = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
				MNIST_Image img;
				MNIST_Label lbl;

				int socket_desc, client_sock, c, read_size;

				if(connection(&socket_desc, server)) exit(1);

					while(1) {
						listen(socket_desc, 3);

						puts("waiting for connections...");
						c = sizeof(struct sockaddr_in);

						// accept connection from an incoming client
						client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*) &c);
						if(client_sock < 0)	{
							perror("connection failed");
							return 1;
						}

						puts("connection successful");

						int image_size = 0;
						int start = 0;
						int how_much = PACKET_SIZE;

						// receive a message from client
						while((read_size = recv(client_sock, client_message + start, how_much, 0)) > 0) {
							printf("received a packet (size: %d)\n", read_size);

							if(read_size == PACKET_SIZE) {
								packet *pkt = (packet *)client_message;
								printf("header %d %d %d\n", pkt->head.ID, pkt->head.length, pkt->head.type);

								// show the image that i have received
								for(int i = 0; i < pkt->head.length; i++) {
									img.pixel[i] = (float) pkt->payload[i];
								}

								lbl = (uint8_t) pkt->head.ID;

								total_case++;
								startTime = clock();
								passIL(img.pixel, intervec1);
								passHL2(intervec1, intervec2);
								passHL3(intervec2, intervec3);
								passHL4(intervec3, intervec4);
								passOL(intervec4, intervec5);
								endTime = clock();
							
								float maxVal = intervec5[0];
								pred = 0;

								for(int i = 1; i < OUTPUT_LENGTH; i++) {
									if(maxVal < intervec5[i]) {
										pred = i;
										maxVal = intervec5[i];
									}
								}

								if(pred == (int) lbl) correct = 1;
								else correct = 0;

								if(correct) {
									printf("[ correct ] ");
									correct_case++;
								}
								else {
									printf("[incorrect] ");
								}
								
								printf("pred: %d, actual: %d / ", pred,lbl);

								gap = (float) (endTime - startTime) / CLOCKS_PER_SEC;
								printf("elapsed time: %fs accuracy: %f\n", gap, (float) correct_case / total_case);

								start = 0;
								how_much = PACKET_SIZE;

							} else {
								start = read_size;
								how_much = PACKET_SIZE - read_size;
							}
						}

						if(read_size == 0) {
							puts("client disconnected");
							fflush(stdout);
						} else if(read_size == -1) {
							perror("recv failed");
						}
					}

				break;
			}
			case 2:
			{
				// init image indices for SGD
				int indices[N_TRAIN_IMAGES];
				for(int i = 0; i < N_TRAIN_IMAGES; i++) {
					indices[i] = i;
				}
				
				correct = 0; total_time = 0; avg_time = 0;
				parameterInitialization_uniform();
				setLearningRate(0.0001);
				startTime = clock(); endTime = 0;

				printf("learning started\n");
				for(int epoch = 0; epoch < TRAINING_EPOCH; epoch++) {

					shuffle(indices, N_TRAIN_IMAGES);

					time_t epoch_startTime = clock();
					FILE *trainingimageFile, *traininglabelFile;
					trainingimageFile = openMNISTImageFile("../data/train-images-idx3-ubyte");
					traininglabelFile = openMNISTLabelFile("../data/train-labels-idx1-ubyte");
					correct_case = total_case = 0;
					float loss = 0;
					float batch_loss = 0;

					for(int imgCount = 0; imgCount < N_TRAIN_IMAGES - N_TEST_IMAGES; imgCount++) {
						total_case++;
						MNIST_Image img = getImage(trainingimageFile, indices[imgCount]);
						MNIST_Label lbl = getLabel(traininglabelFile, indices[imgCount]);

						passIL(img.pixel, intervec1);
						passHL2(intervec1, intervec2);
						passHL3(intervec2, intervec3);
						passHL4(intervec3, intervec4);
						passOL(intervec4, intervec5);
						softmax(intervec5, est);
									
						float maxVal = est[0];
						pred = 0;
						
						for(int i = 1; i < OUTPUT_LENGTH; i++) {
							if(maxVal < est[i]) {
								maxVal = est[i];
								pred = i;
							}
						}

						getError(est, (int)lbl, e);
						
						// backpropagation
						backOL(e, intervec4, est);  
						backHL4(e, intervec3, intervec4, EH1);
						backHL3(EH1, intervec2, intervec3, EH2);
						backHL2(EH2, intervec1, intervec2, EH3);
						backHL1(EH3, img.pixel, intervec1);
						
						if(pred == lbl)	correct = 1;
						else correct = 0;

						if(correct) {
							correct_case++;
						}

						loss += (getLoss(est, lbl) / (N_TRAIN_IMAGES - N_TEST_IMAGES));
						batch_loss += (getLoss(est, lbl) / TRAINING_BATCH);
						/*
						if(imgCount % 1000 == 0) {
							if ((float) correct_case / total_case > 0.65 && getLearningRate() == 0.001) {
								setLearningRate(0.0001);
							}
						}
						*/
						if((imgCount + 1) % TRAINING_BATCH == 0) {
							endTime = clock();
							gap = (float) (endTime - startTime) / CLOCKS_PER_SEC;
							printf("[%d/%d] loss: %f / accuracy: %f / elapsed time: %fs\n", imgCount + 1, N_TRAIN_IMAGES - N_TEST_IMAGES, batch_loss, (float) correct_case / total_case, gap);
							batch_loss = 0;
							correct_case = total_case = 0;
						}
					} // epoch end
					
					// validation
					correct_case = total_case = 0;
					for(int imgCount = N_TRAIN_IMAGES - N_TEST_IMAGES; imgCount < N_TRAIN_IMAGES; imgCount++) {
						total_case++;
						MNIST_Image img = getImage(trainingimageFile, indices[imgCount]);
						MNIST_Label lbl = getLabel(traininglabelFile, indices[imgCount]);

						passIL(img.pixel, intervec1);
						passHL2(intervec1, intervec2);
						passHL3(intervec2, intervec3);
						passHL4(intervec3, intervec4);
						passOL(intervec4, intervec5);
									
						float maxVal = intervec5[0];
						pred = 0;
						
						for(int i = 1; i < OUTPUT_LENGTH; i++)	{
							if(maxVal < intervec5[i]) {
								maxVal = intervec5[i];
								pred = i;
							}
						}

						if(pred == lbl)	correct_case++;
					}

					endTime = clock();
					gap = (float) (endTime - epoch_startTime) / CLOCKS_PER_SEC;
					total_time += gap;
					printf("epoch: %d, loss: %f / accuracy: %f / elapsed time: %fs\n", epoch, loss, (float) correct_case / total_case, gap);
					
					if((float)correct_case / total_case > 0.95) {
						setLearningRate(0.00001);
					}
				}

				avg_time = total_time / TRAINING_EPOCH;

				// test
				correct_case = total_case = 0;
				
				FILE *testingimageFile, *testinglabelFile;
				testingimageFile = openMNISTImageFile("../data/t10k-images-idx3-ubyte");
				testinglabelFile = openMNISTLabelFile("../data/t10k-labels-idx1-ubyte");
				
				for(int imgCount = 0; imgCount < N_TEST_IMAGES; imgCount++) {
					total_case++;
					
					MNIST_Image img = getImage(testingimageFile);
					MNIST_Label lbl = getLabel(testinglabelFile);

					startTime = clock();
					passIL(img.pixel, intervec1);
					passHL2(intervec1, intervec2);
					passHL3(intervec2, intervec3);
					passHL4(intervec3, intervec4);
					passOL(intervec4, intervec5);
					endTime = clock();
								
					float maxVal = intervec5[0];
					pred = 0;
					
					for(int i = 1; i < OUTPUT_LENGTH; i++)	{
						if(maxVal < intervec5[i]) {
							maxVal = intervec5[i];
							pred = i;
						}
					}

					if(pred == lbl)	correct_case++;
				}
				
				endTime = clock();
				gap = (float) (endTime - startTime) / CLOCKS_PER_SEC;
				
				printf("\nlearning finished\n");
				printf("accuracy: %f\n", (float) correct_case / total_case);
				printf("total elapsed time: %fs\n", gap);
				printf("average elapsed time per epoch: %fs\n", avg_time);
				
				break;
			}
			case 3:
			{
				correct = 0; startTime = 0; endTime = 0; total_case = 0; correct_case = 0; total_time = 0; avg_time = 0;

				FILE *testingimageFile, *testinglabelFile;
				testingimageFile = openMNISTImageFile("../data/t10k-images-idx3-ubyte");
				testinglabelFile = openMNISTLabelFile("../data/t10k-labels-idx1-ubyte");
				
				for(int imgCount = 0; imgCount < N_TEST_IMAGES; imgCount++) {
						
					total_case++;
					MNIST_Image img = getImage(testingimageFile);
					MNIST_Label lbl = getLabel(testinglabelFile);

					startTime = clock();
					passIL(img.pixel, intervec1);
					passHL2(intervec1, intervec2);
					passHL3(intervec2, intervec3);
					passHL4(intervec3, intervec4);
					passOL(intervec4, intervec5);
					endTime = clock();
								
					float maxVal = intervec5[0];
					pred = 0;
					
					for(int i = 1; i < OUTPUT_LENGTH; i++)	{
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
					printf("elapsed time: %fs accuracy: %f\n", gap, (float) correct_case / total_case);
					total_time += gap;
				}

				avg_time = total_time / N_TEST_IMAGES;
				printf("\ntest finished\n");
				printf("accuracy: %f\n", (float) correct_case / total_case);
				printf("total elapsed time: %fs\n", total_time);
				printf("average elapsed time per image: %fs\n", avg_time);

				break;
			}
			case 4:
			{
				return 0;
			}
			default:
			{
				break;
			}
		}

	}

	return 0;
}
