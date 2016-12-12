#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <time.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include "protocols.h"
using namespace std;

#define PORT 9999
#define SIZE 2048
#define MAX_CONNECTIONS 5

void* handleConnection(void *sock_desc) {
	int sock = *(int*) sock_desc;
	int read_size;
	char *message, client_message[SIZE];
	while((read_size = recv(sock, client_message, SIZE, 0)) > 0) {
		readMessage(client_message);
		write(sock , sendSTATUS(OK) , strlen(sendSTATUS(OK)));
		//write(sock, client_message, strlen(client_message));
		/* OK, update hash table */
	}
	if(read_size == 0) {
		puts("client disconnected");
		fflush(stdout);
	} else if(read_size == -1) {
		perror("receive failed");
	}
	free(sock_desc);
	return NULL;
}

void* processSocketServer(void *) {
	int socket_desc, client_sock, c, *new_sock;
	struct sockaddr_in server, client;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc < 0) {
		perror("Could not create socket");
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	if(bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("bind failed");
		return NULL;
	}

	listen(socket_desc, MAX_CONNECTIONS);
	c = sizeof(struct sockaddr_in);
	puts("Listening...");
	while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
		puts("Connection accepted");
		pthread_t clientThread;
		new_sock = new int[1];
		*new_sock = client_sock;
		if(pthread_create(&clientThread, NULL, handleConnection, (void*) new_sock) < 0) {
			perror("Could not create thread");
		}
	}
	if(client_sock < 0) {
		perror("Accept failed");
	}
	return NULL;
}
