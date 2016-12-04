#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <iostream>
#include <array>
#include <sstream>
#include <math.h>
#include <string>

#include "BiCountSketch.h"
#include "CountMinSketch.h"
#include "Masters.h"
using namespace std;

#define MAX_MASTER 3 /* max: 3 server -> 3 master*/
#define MAX_WORKER 3 /* max; 3 worker */
#define MAX_CONNECTIONS 3

CountMinSketch phase1(8,16); /* MIN */ /* phase 1 */
BiCountSketch phase2(8, 16); /* MIN */ /* phase 2 */ /* 8: number of buckets | 16: hash functions */
CountMinSketch syn_list(10000, 8); /* rule : SYN packets/s */
BloomFilter exsyn_list(10000, 8);


int length;/* number of addresses */
string worker_list[MAX_WORKER]; /* worker  IP*/
string server_list[MAX_MASTER]; /* server IP*/
bool used_address[MAX_WORKER]{false};
bool phase_flag{false};/* Master -> phase 2, 3 */
bool actived_address[MAX_MASTER]{false};
bool attack[MAX_MASTER]{false}; /* attack -> server */
uint32_t exasym_num[MAX_WORKER]{0}; /* not accumulation */
uint32_t exdist_num[MAX_WORKER]{0}; /* slave -> master*/
uint32_t sum_num[MAX_WORKER]{0};/* slave -> master: sum hash row */

Masters master; /* Master: phase 1 -> on/off phase_flag */
int master_index{-1}; /* default: -1 (no master) */



int new_sock[MAX_CONNECTIONS]{0};

uint32_t getIPAddress(const char *addr) {
		int byte[4]{0};
		sscanf(addr, "%d.%d.%d.%d", &byte[0], &byte[1], &byte[2], &byte[3]);
	return (byte[0]*pow(256,3) + byte[1]*pow(256,2) + byte[2]*256 + byte[3]);
}

char* _intoa(unsigned int addr, char* buf, u_short bufLen) {
	char *cp, *retStr;
	u_int byte;
	int n;
	cp = &buf[bufLen];
	*--cp = '\0';
	n = 4;
	do {
		byte = addr & 0xff;
		*--cp = byte % 10 + '0';
		byte /= 10;
		if (byte > 0) {
			*--cp = byte % 10 + '0';
			byte /= 10;
			if (byte > 0)
				*--cp = byte + '0';
		}
		*--cp = '.';
		addr >>= 8;
	} while (--n > 0);
	retStr = (char*) (cp + 1);
	return retStr;
}

char* intoa(unsigned int addr) {
	static char buf[sizeof "fff.fff.fff.fff"];
	return _intoa(addr, buf, sizeof(buf));
}

uint32_t sumList(uint32_t list[]){
	uint32_t sum = 0;
	for (int i = 0; i < MAX_WORKER; i++){
		sum += list[i];
	}
	return sum;
}

#endif /* GLOBALS_H_ */
