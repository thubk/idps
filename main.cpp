#include <iostream>
#include <math.h>
#include <pthread.h>
using namespace std;
#include "CountMinSketch.h"
#include "pfringcpp.cpp"
#include "server.cpp"
#include "client.cpp"
#include "protocols.h"

int main(int argc, char *argv[]) {

//	CountMinSketch cms(5, 128);
//	uint32_t key1 = 3232235525;/*192.168.0.5*/
//	uint32_t key2 = 3232235526;/*192.168.0.6*/
//	uint32_t key3 = 3221225572;/*192.0.0.100*/
//	uint32_t key4 = 3221225482;/*192.0.0.10*/
//	uint64_t key5 = (uint64_t(key1) << 32) + uint64_t(key3);
//	uint64_t key6 = (uint64_t(key2) << 32) + uint64_t(key4);
//
//
//	cms.process(&key2, &key6, &key4, sizeof(uint32_t), key2);
//	cms.process(&key2, &key6, &key4, sizeof(uint32_t), key2);
//	cms.process(&key1, &key5, &key3, sizeof(uint32_t), key1);
//	for (uint32_t i = 1; i < 10000; i++) {
//		key4 += i;
//		uint64_t temp = (uint64_t(key1) << 32) + uint64_t(key4);
//		cms.process(&key1, &temp, &key4, sizeof(uint32_t), key1);
//	}
//	cout << cms.getDistNumBCS(&key1, sizeof(uint32_t), key1) << endl;
//	cout << cms.getDistNumBCS(&key2, sizeof(uint32_t), key2) << endl;
	pthread_t thread[3];
	pthread_create(&thread[0], NULL, processSocketServer, NULL);
	pthread_create(&thread[1], NULL, startPFring, NULL);
	sleep(10);
	pthread_create(&thread[2], NULL, processSocketClient, NULL);
	while (1);

	return 0;
}
