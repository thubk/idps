#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <iostream>
#include <array>
#include <sstream>
#include <math.h>
#include <string>

#include "BiCountSketch.h"
#include "CountMinSketch.h"
using namespace std;

/* 8: number of bucket << | 16: hash function */
BiCountSketch bcs(8, 16);
CountMinSketch cms(8,16);

#define MAX_MASTER 3 /* max: 3 server */

uint8_t length;/* number of addresses */
string address[MAX_MASTER];
string server_list[MAX_MASTER];
bool used_address[MAX_MASTER];


/* 65535 buckets, 8 hash functions */

uint32_t getIPAddress(const char *addr) {
		int byte[4]{0};
		sscanf(addr, "%d.%d.%d.%d", &byte[0], &byte[1], &byte[2], &byte[3]);
	return (byte[0]*pow(256,3) + byte[1]*pow(256,2) + byte[2]*256 + byte[3]);
}

char* _intoa(unsigned int addr, char* buf, u_short bufLen) {
	char *cp, *retStr;
	u_int byte;
	;
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

#endif /* GLOBALS_H_ */
