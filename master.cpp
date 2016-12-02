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
#include <stdio.h>
#include <errno.h>
#include "protocols.h"
#include "globals.h"
using namespace std;

void* handleConnection(void *sock_desc) {
	int sock = *(int*) sock_desc;
	int read_size;
	char *message, buffer[BUFFER_SIZE];
	while((read_size = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
		puts(buffer);
		std::array<uint32_t, 4> values = readMessage(buffer);
		/*udpate hash table */


		memset(&buffer, 0, sizeof(buffer));
		if(values[3] == 0) {
			write(sock , sendSTATUS(NO) , strlen(sendSTATUS(NO)));
		} else {
			write(sock , sendSTATUS(OK) , strlen(sendSTATUS(OK)));
		}
	}
	if(read_size == 0) {
		puts("Master: client disconnected");
		fflush(stdout);
	} else if(read_size == -1) {
		perror("Master: receive failed");
	}
	free(sock_desc);
	return NULL;
}

void* processSocketServer(void *) {
	int socket_desc, client_sock, c, *new_sock;
	struct sockaddr_in server, client;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc < 0) {
		perror("Master: could not create socket");
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(MASTER_PORT);

	if(bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("Master: bind failed");
		return NULL;
	}

	listen(socket_desc, MAX_CONNECTIONS);
	c = sizeof(struct sockaddr_in);
	puts("Master: listening...");
	while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
		puts("Master: connection accepted");
		pthread_t clientThread;
		new_sock = new int[1];
		*new_sock = client_sock;
		if(pthread_create(&clientThread, NULL, handleConnection, (void*) new_sock) < 0) {
			perror("Master: could not create thread");
		}
		//pthread_join(clientThread, NULL);
	}
	if(client_sock < 0) {
		perror("Master: accept failed");
	}
	return NULL;
}
