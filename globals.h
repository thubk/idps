#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <array>
#include <string>
#include <string.h>
#include <sstream>

#include "BidirectionalCountSketch.h"

#define MASTER_PORT 9999
#define DEFAULT_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 2048
#define TIMEINTERVAL_CLIENT 5
#define TIMEINTERVAL_START 5
#define MAX_CONNECTIONS 5
#define MAX_THREAD 5
#define MAX_MASTER 10 /* max: 10 server */

uint8_t length;/* number of addresses */
string address[MAX_MASTER];
string server_list[MAX_MASTER];
bool used_address[MAX_MASTER];


/* 65535 buckets, 8 hash functions */
BidirectionalCountSketch bcs(65535, 8);

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
