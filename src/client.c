#include "client.h"

#define BUFFERT 1024

int main(int argc, char **argv) {
	struct timeval start, stop, delta;
	int socket_file_descriptor, file_descriptor;
	char buf[BUFFERT];
	off_t total_num_of_bytes = 0, sent_char_num, sz;
	long int read_bytes_num;
	unsigned int address_len = sizeof(struct sockaddr_in);
	struct stat buffer;

	if (argc != 4) {
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n", argv[0]);

		return EXIT_FAILURE;
	}

	// args
	char *IP_SERV = argv[1];
	char *PORT_SERV = argv[2];
	char *FILENAME = argv[3];

	socket_file_descriptor = create_client_socket(atoi(PORT_SERV), IP_SERV);

	if ((file_descriptor = open(FILENAME, O_RDONLY)) == -1) {
		perror("open fail");

		return EXIT_FAILURE;
	}

	// file size
	if (stat(FILENAME, &buffer) == -1) {
		perror("stat fail");

		return EXIT_FAILURE;
	} else {
		sz = buffer.st_size;
	}

	// preparation of the send
	bzero(&buf, BUFFERT);

	if (connect(socket_file_descriptor, (struct sockaddr *)&sock_serv_client, address_len) == -1) {
		perror("conexion error\n");
		exit(3);
	}

	gettimeofday(&start, NULL);
	read_bytes_num = read(file_descriptor, buf, BUFFERT);
	while (read_bytes_num) {
		if (read_bytes_num == -1) {
			perror("read fails");

			return EXIT_FAILURE;
		}
		sent_char_num = sendto(socket_file_descriptor, buf, read_bytes_num, 0, (struct sockaddr *)&sock_serv_client, address_len);

		if (sent_char_num == -1) {
			perror("send error");

			return EXIT_FAILURE;
		}
		total_num_of_bytes += sent_char_num;
		// fprintf(stdout,"----\n%s\n----\n",buf);
		bzero(buf, BUFFERT);
		read_bytes_num = read(file_descriptor, buf, BUFFERT);
	}
	// read has returned 0: end of file
	// to unblock the server
	sent_char_num = sendto(socket_file_descriptor, buf, 0, 0, (struct sockaddr *)&sock_serv_client, address_len);
	gettimeofday(&stop, NULL);
	duration(&start, &stop, &delta);

	printf("Number of bytes transferred : %ld\n", total_num_of_bytes);
	printf("On a total size of : %ld \n", sz);
	printf("For a total duration of : %ld.%ld \n", delta.tv_sec, delta.tv_usec);

	close(socket_file_descriptor);
	
	return EXIT_SUCCESS;
}