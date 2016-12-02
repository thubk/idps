#include <iostream>
#include <fstream>
#include <math.h>
#include <pthread.h>
#include "pfringcpp.cpp"
#include "master.cpp"
#include "slave.cpp"
#include "globals.h"
using namespace std;

#define DEFAULT_ADDRESS "127.0.0.1"
#define TIMEINTERVAL_START 5
#define MAX_THREAD 5


int main(int argc, char *argv[]) {
	pthread_t thread[MAX_THREAD];
	string len;
	string line;
	ifstream infile("config");
	getline(infile, len);
	length = atoi(len.c_str());
	uint8_t i = 0;
	while (getline(infile, line)) {
		string addr = line;
		size_t f = addr.find(":");
		address[i] = addr.substr(0, f);
		server_list[i] = addr.substr(f+1);
		i++;
	}
	pthread_create(&thread[length], NULL, startPFring, NULL);
	for (uint8_t n = 0; n < length; n++) {
		if (address[n] == DEFAULT_ADDRESS) {
			used_address[n] = true; /* using */
			pthread_create(&thread[n], NULL, processSocketServer, NULL);
		} else {
			sleep(TIMEINTERVAL_START);
			pthread_create(&thread[n], NULL, processSocketClient, NULL);
		}
	}
	while (1);
	return 0;
}
