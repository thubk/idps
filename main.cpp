#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <typeinfo>
#include "CountMinSketch.h"
#include "pfringcpp.cpp"
#include "globals.h"
#include "master.cpp"
#include "slave.cpp"
using namespace std;

int main(int argc, char *argv[]) {
//	pthread_t thread[MAX_THREAD];
//	string len;
//	string line;
//	ifstream infile("config");
//	getline(infile, len);
//	length = atoi(len.c_str());
//	uint8_t i = 0;
//	while (getline(infile, line)) {
//		string addr = line;
//		size_t f = addr.find(":");
//		address[i] = addr.substr(0, f);
//		server_list[i] = addr.substr(f+1);
//		i++;
//	}
//	pthread_create(&thread[length], NULL, startPFring, NULL);
//	for (uint8_t n = 0; n < length; n++) {
//		if (address[n] == DEFAULT_ADDRESS) {
//			used_address[n] = true; /* using */
//			pthread_create(&thread[n], NULL, processSocketServer, NULL);
//		} else {
//			sleep(TIMEINTERVAL_START);
//			pthread_create(&thread[n], NULL, processSocketClient, NULL);
//		}
//	}
//	while (1);

	cout << "ngo minh nhat ";
//	CountMinSketch cms(65535, 8);
//	uint32_t key = 3232235620;
//	cms.process(&key,sizeof(uint32_t), key);
//	cout << cms.contain(&key, sizeof(uint32_t), key);
	return 0;
}
