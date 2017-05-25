/*
 * networking.h
 *
 *  Created on: May 24, 2017
 *      Author: root
 */

#ifndef SRC_NETWORK_NETWORKING_H_
#define SRC_NETWORK_NETWORKING_H_

#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<stdint.h>

#define PACKET_SIZE 1596
#define MAX_DATA 756

typedef struct header{
	uint32_t type;
	uint32_t length;
	uint32_t ID;
} header;

typedef struct packet{
	header head;
	uint8_t payload[MAX_DATA];
} packet;

int connection(int *socket_desc, struct sockaddr_in *server);




#endif /* SRC_NETWORK_NETWORKING_H_ */
