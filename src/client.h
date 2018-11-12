#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

struct sockaddr_in sock_serv_client;

/* 
 * Function allowing the creation of a socket
 * Returns a file descriptor
 */
int create_client_socket(int port, char *ipaddr) {
	int address_len;
	int socket_file_descriptor;

	socket_file_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_file_descriptor == -1) {
		perror("socket fail");
		
		return EXIT_FAILURE;
	}

	// preparation of the address of the destination socket
	address_len = sizeof(struct sockaddr_in);
	bzero(&sock_serv_client, address_len);

	sock_serv_client.sin_family = AF_INET;
	sock_serv_client.sin_port = htons(port);
	if (inet_pton(AF_INET, ipaddr, &sock_serv_client.sin_addr) == 0) {
		printf("Invalid IP adress\n");

		return EXIT_FAILURE;
	}

	return socket_file_descriptor;
}

#endif