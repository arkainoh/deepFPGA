/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include "mnist_utils/mnist_utils.h"
#include <stdint.h>

#define MAX_DATA 1500

typedef struct header{
	uint32_t type;
	uint32_t length;
	uint32_t ID;
} header;

typedef struct packet{
	header head;
	uint8_t payload[MAX_DATA];
} packet;



int main(int argc , char *argv[])
{
    int sent;
    int i;
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    FILE *imageFile, *labelFile;
    int imgCount = 0;
    packet pkt;
    imageFile = openMNISTImageFile("test/t10k-images-idx3-ubyte");
    labelFile = openMNISTLabelFile("test/t10k-labels-idx1-ubyte");   

    

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("10.0.0.12");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
   imgCount = 0; 
    for (imgCount=0; imgCount<1000; imgCount++){
	
        MNIST_Image img = getImage(imageFile);
	MNIST_Label lbl = getLabel(labelFile);

	pkt.head.length = 785;
	pkt.head.type = 12;
	pkt.head.ID = lbl;
	for(i=0;i<785;i++)
	{
	    pkt.payload[i] = (unsigned char) img.pixel[i];

	}
	
          //Send some data
        if( sent = send(sock ,&pkt , 1596 , 0) < 0)
        {

            puts("Send failed");
            return 1;
        }
         
	    printf("data sent :%d\n",sent);
  
	         
    }
     
    close(sock);
    return 0;
}
