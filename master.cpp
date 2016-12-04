#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include "protocols.h"
#include "globals.h"
using namespace std;

#define BUFFER_SIZE 2048
#define MASTER_PORT 9999
#define TIMEINTERVAL_MASTER 1
int getIndex(int sock) {/* extra */
	int index = -1;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (new_sock[i] == sock) {
			index = i;
			break;
		}
	}
	return index;
}

void* handleConnection(void *sock_desc) {
	uint32_t server_key = getIPAddress(server_list[master_index].c_str());/* default */
	int nthHash = phase2.getHashIndexBucket(&server_key, sizeof(uint32_t),
			server_key);
	int sock = *(int*) sock_desc;
	int index = getIndex(sock);
	int read_size;
	char *message, buffer[BUFFER_SIZE];
	while (1) {
		sleep(TIMEINTERVAL_MASTER);
		if (phase_flag) {
			/* request phase 1,2,3 > client */
			if (attack[master_index]) {
				message = sendMessage(0,2,nthHash,1,master_index);/* hash index: 0-> index */
			} else {
				message = sendMessage(0,2,nthHash,0,master_index);/* hash index: 0-> index */
			}
			write(sock, message, strlen(message));
			memset(message, 0, sizeof(message));
		} else {
			message = sendMessage(0,1,0,0,master_index);
			write(sock, message, strlen(message));
			memset(message, 0, sizeof(message));
		}
		if ((read_size = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
			std::array<uint32_t, 5> values = readMessage(buffer);
			int code = values[0];
			switch (code) {
			case 1:
				/* update phase 1 */
				puts("Phase 1: Slave -> Master");
				puts(buffer);
				phase1.updateCounter(&server_key, sizeof(uint32_t), (uint32_t) values[1]);
				break;
			case 2:
				/* update phase 2,3 */
				puts("Phase 2: Slave -> Master");
				puts(buffer);
				phase1.updateCounter(&server_key, sizeof(uint32_t), (uint32_t) values[1]);
				exasym_num[index] = values[2];
				exdist_num[index] = values[3];
				sum_num[index] = values[4];
				break;
			default:
				perror("not support");
				break;
			}
		}
	}
	if (read_size == 0) {
		puts("Master: client disconnected");
		fflush(stdout);
	} else if (read_size == -1) {
		perror("Master: receive failed");
	}
	free(sock_desc);
	return NULL;
}

void* processSocketServer(void *) {
	pthread_t clientThread[MAX_CONNECTIONS];
	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_desc < 0) {
		perror("Master: could not create socket");
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(MASTER_PORT);

	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("Master: bind failed");
		return NULL;
	}

	listen(socket_desc, MAX_CONNECTIONS);
	c = sizeof(struct sockaddr_in);
	puts("Master: listening...");
	int i = 0;
	while ((client_sock = accept(socket_desc, (struct sockaddr *) &client,
			(socklen_t*) &c))) {
		puts("Master: connection accepted");
		new_sock[i] = client_sock;
		if (pthread_create(&clientThread[i], NULL, handleConnection,
				(void*) &new_sock[i]) < 0) {
			perror("Master: could not create thread");
		}
		i++;
	}
	if (client_sock < 0) {
		perror("Master: accept failed");
	}
	return NULL;
}
