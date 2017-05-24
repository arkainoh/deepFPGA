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

int connection(int *socket_desc, struct sockaddr_in *server);




#endif /* SRC_NETWORK_NETWORKING_H_ */
