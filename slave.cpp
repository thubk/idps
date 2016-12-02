#include <iostream>
#include <stdio.h>
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
#include <string>
#include "protocols.h"
#include "globals.h"
using namespace std;
/* n slave -> n server -> n thread */
#define TIMEINTERVAL_CLIENT 5
#define BUFFER_SIZE 2048
#define MASTER_PORT 9999

int getMyAddress(){
	int index = -1;
	for(int i = 0; i < length; i++){
		if(!used_address[i]){
			index = i;
			used_address[i] = true;
			break;
		}
	}
	return index;
}

void* processSocketClient(void *) {
	int index = getMyAddress();
	if( index < 0) {
		perror("Client: Server'IP list failed");
	}
	const char *ip = address[index].c_str();
	uint32_t key = getIPAddress(server_list[index].c_str()); /* Server'IP -> Hash Key */
	puts("Master'IP: ");
	puts(ip);
	int connection;
	struct hostent *serverAddress;
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
		sleep(TIMEINTERVAL_CLIENT);
		/* read data form hash table -> send */
		message = "<msg><count>888</count><freq>100</freq><asym>777</asym><dist>10000</dist></msg>";
		if (send(sock, message, strlen(message), 0) < 0) {
			perror("Client: send failed");
			return NULL;
		}
		puts("Slave:->");
		puts(ip);
		puts(message);
		memset(&message, 0, sizeof(message));
		if (recv(sock, buffer, BUFFER_SIZE, 0) < 0) {
			perror("Client: receive failed");
			return NULL;
		}
		puts(ip);
		puts("->Slave:");
		puts(buffer);
		/* repeat -> NO */
		while (!readStatus(buffer)) {
			puts(buffer);
			if (send(sock, message, strlen(message), 0) < 0) {
				perror("Slave: send failed");
				return NULL;
			}
			if (recv(sock, buffer, BUFFER_SIZE, 0) < 0) {
				perror("Slave: receive failed");
				return NULL;
			}
		}
	}
	close(sock);
	return NULL;
}

