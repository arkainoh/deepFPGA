#include "networking.h"

int connection(int *socket_desc, struct sockaddr_in *server) {
	
	*socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (*socket_desc == -1)	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server->sin_addr.s_addr = inet_addr("10.0.0.12");
	server->sin_family = AF_INET;
	server->sin_port = htons( 8888 );

	if( bind(*socket_desc,(struct sockaddr *)server , sizeof(*server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");

	return 0;
}
