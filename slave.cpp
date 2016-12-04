#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <array>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string>
#include "protocols.h"
#include "globals.h"
using namespace std;
/* n slave -> n server -> n thread */
#define TIMEINTERVAL_SLAVE 5
#define BUFFER_SIZE 2048
#define MASTER_PORT 9999

int getMyAddress() {
	int index = -1;
	for (int i = 0; i < length; i++) {
		if (!used_address[i]) {
			index = i;
			used_address[i] = true;
			break;
		}
	}
	return index;
}

void* processSocketClient(void *) {
	int index = getMyAddress();
	if (index < 0) {
		perror("Client: Server'IP list failed");
	}
	const char *ip = worker_list[index].c_str();
	uint32_t key = getIPAddress(server_list[index].c_str()); /* Server'IP -> Hash Key */
	puts("Master'IP: ");
	puts(ip);
	int connection;
	struct sockaddr_in server;
	int sock;
	char *message, buffer[BUFFER_SIZE];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Slave: cannot open socket");
	}
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(MASTER_PORT);

	connection = connect(sock, (struct sockaddr *) &server, sizeof(server));

	if (connection < 0) {
		perror("Slave: connect failed");
		return NULL;
	}
	puts("Slave: connection successful");
	while (1) {
		if (recv(sock, buffer, BUFFER_SIZE, 0) > 0) {
			std::array<uint32_t, 5> values = readMessage(buffer);
			int code = values[0];
			puts("Master -> Slave");
			puts(buffer);
			switch (code) {
			case 0:
				if (values[1] == 1) {
					if(values[3] == 0) {
						attack[values[4]] = false; /* normal */
					}
					uint32_t freq = phase1.getMinCounter(&key, sizeof(uint32_t));
					message = sendMessage(1,freq,0,0,0);
					send(sock, message, strlen(message), 0);
					/* reset freq */
					phase1.resetCounter(&key, sizeof(uint32_t), freq);
					memset(message, 0, sizeof(message));
					/* send counter: phase 1 */
				}
				if (values[1] == 2) {
					if(values[3] == 1){
						attack[values[4]] = true; /* attack */
					}
					uint32_t freq = phase1.getMinCounter(&key, sizeof(uint32_t));
					uint32_t asym = phase2.getMinFrequence(&key, sizeof(uint32_t));
					uint32_t distnum = phase2.getDistNumBCS(&key, sizeof(uint32_t), key);
					uint32_t sum = phase2.getSumHashesRow((int)values[2]);
					message = sendMessage(2,freq,asym,distnum,sum);
					send(sock, message, strlen(message), 0);
					/*  reset freq */
					phase1.resetCounter(&key, sizeof(uint32_t), freq);
					memset(message, 0, sizeof(message));
					/* send asymmetric: phase 2 */
				}
				break;
			case 3:
				if (values[1] == 1) {/* status: NO -> send again  phase 1*/
					puts("Phase 1: NO");
				}
				if (values[1] == 2) {/* status: NO -> send again  phase 2*/
					puts("Phase 2: NO");
				}
				break;
			default:
				perror("not support");
				break;
			}
		}
	}
	close(sock);
	return NULL;
}

