#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

struct sockaddr_in sock_serv_server, sock_clt;

/*
 * Function allowing the creation of a socket and its attachment to the system
 * Returns a file descriptor in the process descriptor table
 * bind allows its definition in the system
 */
int create_server_socket(int port) {
    socklen_t address_len;
    int socket_file_descriptor;
    int yes = 1;

    socket_file_descriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        perror("socket fail");

        return EXIT_SUCCESS;
    }

    /* SOL_SOCKET: To manipulate options at the API level sockets
     * SO_REUSEADDR: When you have to restart a server after a crash it can serve
     * to not have an error when creating the socket (the IP stack of the system does not have
     * always had time to clean).
     * When multiple servers listening to the same port ... (?)
     */
    if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt erreur");
        exit(5);
    }

    // preparation of the address of the destination socket
    address_len = sizeof(struct sockaddr_in);
    bzero(&sock_serv_server, address_len);

    sock_serv_server.sin_family = AF_INET;
    sock_serv_server.sin_port = htons(port);
    sock_serv_server.sin_addr.s_addr = htonl(INADDR_ANY);

    // Assign an identity to the socket
    if (bind(socket_file_descriptor, (struct sockaddr *)&sock_serv_server, address_len) == -1) {
        perror("bind fail");
        
        return EXIT_FAILURE;
    }

    return socket_file_descriptor;
}

#endif