/*
 * main.cpp
 *
 *  Created on: May 21, 2017
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include<unistd.h>    //write


#include "mnist_utils/mnist-utils.h"
#include "mnist_utils/mnist_feedforward.h"

#include "network/networking.h"

int main(int argc, char *argv[]) {

	float intervec1[HL_LENGTH];
	float intervec2[HL_LENGTH];
	float intervec3[HL_LENGTH];
	float intervec4[HL_LENGTH];

	int correct = 0;
	float gap;
	int pred;
	time_t startTime = 0, endTime = 0;
	int total_case=0;
	int correct_case=0;


	while (true)
	{
		printf("please, enter the option, \n1: testing with /test/t10kblahblah data\n	\r \
				2: server-client model training visualization\n\r\
				3: accuracy control\n\r\
				4: exit\n\r");
		int where;
		std::cin >> where;
		switch(where){

		case 1:
		{
			correct = 0; startTime = 0; endTime = 0; total_case = 0; correct_case = 0;

			FILE *imageFile, *labelFile;
			imageFile = openMNISTImageFile("test/t10k-images-idx3-ubyte");
			labelFile = openMNISTLabelFile("test/t10k-labels-idx1-ubyte");

			for (int imgCount=0; imgCount<MNIST_MAX_TRAINING_IMAGES; imgCount++){
				total_case++;
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
			correct = 0; startTime = 0; endTime = 0; total_case = 0; correct_case = 0;
		    char client_message[2000];
			struct sockaddr_in *server =(struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
			struct sockaddr_in *client =(struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

		    int socket_desc , client_sock , c , read_size;

			if( connection(&socket_desc, server))
		    	exit(1);

		    while(1)
		    {
		    	//listen
		        listen(socket_desc , 3);

		        //Accept and incoming connection
		        puts("Waiting for incoming connections...");
		        c = sizeof(struct sockaddr_in);

		        //accept connection from an incoming client
		        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		        if (client_sock < 0)
		        {
		            perror("accept failed");
		            return 1;
		        }
		        puts("Connection accepted");
	        	int image_size = 0;
		        //Receive a message from client
		        while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
		        {
		        	{
			            write(client_sock , client_message , strlen(client_message));
		        	}
		        }

		        if(read_size == 0)
		        {
		            puts("Client disconnected");
		            fflush(stdout);
		        }
		        else if(read_size == -1)
		        {
		            perror("recv failed");
		        }

		    }

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
