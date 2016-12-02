#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include "protocols.h"

/* n slave -> n server -> n thread */
#define SERVER_ADDRESS "127.0.0.1" /* read config */
#define SERVER_PORT 9999
#define SIZE 2048

void* processSocketClient(void *){
	int connection;
	struct hostent *serverAddress;
	struct sockaddr_in server;
	int sock;
	char *message, server_reply[SIZE];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		perror("Cannot open socket");
	}
	server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	connection = connect(sock, (struct sockaddr *)&server, sizeof(server));
	if(connection < 0) {
		perror("Connect failed");
		return NULL;
	}
	puts("Connection successful: Client");
	while(1) {
		message = "<msg><freq>100</freq><asym>777</asym><dist>10000</dist></msg>";/* message */
		if(send(sock, message, strlen(message), 0) < 0) {
			perror("send failed");
			return NULL;
		}
		if(recv(sock, server_reply, SIZE, 0) < 0) {
			perror("receive failed");
			return NULL;
		}
		puts("Server reply");
		puts(server_reply);
		sleep(10);
	}
	close(sock);
	return NULL;
}

