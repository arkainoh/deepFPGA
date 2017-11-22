#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#include "mnist_utils/mnist_utils.h"
#include "mnist_utils/mnist_nn.h"
#include "network/networking.h"
#include "parameters/parameters.h"
#define TRAINING_EPOCH 20
#define TRAINING_BATCH 1000

int main(int argc, char *argv[]) {

	float intervec1[HL_LENGTH];
	float intervec2[HL_LENGTH];
	float intervec3[HL_LENGTH];
	float intervec4[HL_LENGTH];
	float intervec5[OUTPUT_LENGTH];

	float EH1[HL_LENGTH];
	float EH2[HL_LENGTH];
	float EH3[HL_LENGTH];
	float EH4[HL_LENGTH];
	float e[OUTPUT_LENGTH];
	float t[OUTPUT_LENGTH];	

	int correct;
	float gap;
	int pred;
	time_t startTime, endTime;
	int total_case;
	int correct_case;

	while (true)
	{
		printf("\r\n\
				<MENU>\r\n\
				1: connect to client\n\r\
				2: train\n\r\
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
								// showImg(img.pixcel);

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
								for(int i = 1; i < 10; i++) {
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
								printf("elapsed time: %dm %ds accuracy: %f\n", (int) gap / 60, (int) gap % 60, (float) correct_case / total_case);

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
				correct = 0; total_case = 0; correct_case = 0;
				
				weightInitialization_uniform();
				setLearningRate(0.001);
				startTime = clock(); endTime = 0;

				for(int epoch = 0; epoch < TRAINING_EPOCH; epoch++) {
					
					FILE *trainingimageFile, *traininglabelFile;
					trainingimageFile = openMNISTImageFile("../data/train-images-idx3-ubyte");
					traininglabelFile = openMNISTLabelFile("../data/train-labels-idx1-ubyte");

					float loss = 0;
					for(int imgCount = 0; imgCount < MNIST_MAX_TRAINING_IMAGES; imgCount++) {
						total_case++;
						MNIST_Image img = getImage(trainingimageFile);
						MNIST_Label lbl = getLabel(traininglabelFile);

						passIL(img.pixel, intervec1);
						passHL2(intervec1, intervec2);
						passHL3(intervec2, intervec3);
						passHL4(intervec3, intervec4);
						passOL(intervec4, intervec5);
									
						float maxVal = intervec5[0];
						pred = 0;
						
						for(int i = 1; i < 10; i++) {
							if(maxVal < intervec5[i]) {
								maxVal = intervec5[i];
								pred = i;
							}
						}

						getError(intervec5,(int) lbl, e);
						
						// backpropagation
						backOL(e, intervec4, intervec5);  
						backHL4(e, intervec3, intervec4, EH1);
						backHL3(EH1, intervec2, intervec3, EH2);
						backHL2(EH2, intervec1, intervec2, EH3);
						backHL1(EH3, img.pixel, intervec1);
						
						if(pred == lbl)	correct = 1;
						else correct = 0;

						if(correct) {
							correct_case++;
						}

						loss += (getLoss(intervec5, lbl) / MNIST_MAX_TRAINING_IMAGES);

						if(imgCount % TRAINING_BATCH == 0) {

							endTime = clock();
							gap = (float) (endTime - startTime) / CLOCKS_PER_SEC;
							printf("[%d/%d] loss: %f / accuracy: %f / elapsed time: %dm %ds\n", imgCount, MNIST_MAX_TRAINING_IMAGES, loss, (float) correct_case / total_case, (int) gap / 60, (int) gap % 60);

							if ((float) correct_case / total_case > 0.65) {
								setLearningRate(0.0001);
							}
						}
					}
					endTime = clock();
					gap = (float) (endTime - startTime) / CLOCKS_PER_SEC;
					printf("epoch: %d, loss: %f / accuracy: %f / elapsed time: %dm %ds\n", epoch, loss, (float) correct_case / total_case, (int) gap / 60, (int) gap % 60);
				}
				break;
			}
			case 3:
			{
				correct = 0; startTime = 0; endTime = 0; total_case = 0; correct_case = 0;

				FILE *testingimageFile, *testinglabelFile;
				testingimageFile = openMNISTImageFile("../data/t10k-images-idx3-ubyte");
				testinglabelFile = openMNISTLabelFile("../data/t10k-labels-idx1-ubyte");
				for(int imgCount = 0; imgCount < MNIST_MAX_TESTING_IMAGES; imgCount++) {
													
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
					printf("elapsed time: %dm %ds accuracy: %f\n", (int) gap / 60, (int) gap % 60, (float) correct_case/total_case);

				}
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
