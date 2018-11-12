#include "server.h"

#define BUFFERT 1024

/* Size of the customer queue */
#define BACKLOG 1

typedef uint16_t ushort;

int main(int argc, char **argv) {
    int socket_file_descriptor, file_descriptor;
    unsigned int address_len = sizeof(struct sockaddr_in);
    long int received_bytes_num, write_in_bytes_num, total_num_of_bytes = 0;
    unsigned int accept_file_descriptor;
    ushort clt_port;
    char buffer[BUFFERT], filename[256];
    char dst[INET_ADDRSTRLEN];

    // Variable for the date
    time_t intps;
    struct tm *tmi;

    if (argc != 2) {
        perror("Error usage : <port_serv>\n");
        exit(3);
    }

    char *IP_SERV = argv[1];
    socket_file_descriptor = create_server_socket(atoi(IP_SERV));
    printf("server listening on %d...\n", atoi(IP_SERV));

    bzero(buffer, BUFFERT);
    listen(socket_file_descriptor, BACKLOG);

    // Function that strikes the function connects the client
    accept_file_descriptor = accept(socket_file_descriptor, (struct sockaddr *)&sock_clt, &address_len);
    if (accept_file_descriptor == -1) {
        perror("accept fail");
        return EXIT_FAILURE;
    } else {
        if (inet_ntop(AF_INET, &sock_clt.sin_addr, dst, INET_ADDRSTRLEN) == NULL) {
            perror("erreur socket");
            exit(4);
        }
        clt_port = ntohs(sock_clt.sin_port);
        printf("Start of the connection for : %s:%d\n", dst, clt_port);

        // Processing the file name with the date
        bzero(filename, 256);
        intps = time(NULL);
        tmi = localtime(&intps);
        bzero(filename, 256);
        sprintf(filename, "clt.%d.%d.%d.%d.%d.%d", tmi->tm_year + 1900, tmi->tm_mon + 1, tmi->tm_mday, tmi->tm_hour, tmi->tm_min, tmi->tm_sec);
        printf("Creating the copied output file : %s\n", filename);

        if ((file_descriptor = open(filename, O_CREAT | O_WRONLY, 0600)) == -1) {
            perror("open fail");
            exit(3);
        }
        bzero(buffer, BUFFERT);

        received_bytes_num = recv(accept_file_descriptor, buffer, BUFFERT, 0);
        while (received_bytes_num) {
            if (received_bytes_num == -1) {
                perror("recv fail");
                exit(5);
            }

            if ((write_in_bytes_num = write(file_descriptor, buffer, received_bytes_num)) == -1) {
                perror("write fail");
                exit(6);
            }
            total_num_of_bytes += write_in_bytes_num;
            bzero(buffer, BUFFERT);
            received_bytes_num = recv(accept_file_descriptor, buffer, BUFFERT, 0);
        }
        close(socket_file_descriptor);
        close(file_descriptor);
    }
    close(accept_file_descriptor);

    printf("End of transmission with %s.%d\n", dst, clt_port);
    printf("Number of bytes received : %ld \n", total_num_of_bytes);

    return EXIT_SUCCESS;
}