#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <iostream>
#include <array>
#include <sstream>
#include <math.h>
#include <string>

#include <CkHttpRequest.h>
#include <CkHttp.h>
#include <CkHttpResponse.h>


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

BloomFilter drop_list(10000, 8); /* drop ip list */


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

void postrules(uint32_t src_ip, int name_flow) {

    CkHttpRequest req;
    CkHttp http;

    bool success;

    success = http.UnlockComponent("THU-NHAT");
    if (success != true) {
        std::cout << http.lastErrorText() << "\r\n";
        return;
    }

    char *ip = intoa(src_ip);
    ostringstream id;
    id << name_flow;
    const char * scrip = (const char *)ip;
    string ipsrc = scrip;
    http.put_AcceptCharset("");
    http.put_UserAgent("");
    http.put_AcceptLanguage("");
    http.put_AllowGzip(false);
    string jsonText ="{\"switch\":\"00:00:00:00:00:00:00:01\",\"name\":\""+id.str()+"\",\"ipv4_src\":\"" + ipsrc + "\",\"cookie\":\"0\",\"priority\":\"1001\",\"in_port\":\"1\",\"eth_type\":\"0x0800\",\"active\":\"true\",\"actions\":\"\"}";
    cout<<jsonText<<endl;
    CkHttpResponse *resp = 0;
    resp = http.PostJson("http://127.0.0.1:8080/wm/staticflowpusher/json",jsonText.c_str());
    if (resp == 0 ) {
        std::cout << http.lastErrorText() << "\r\n";
    }
    else {
        std::cout << resp->bodyStr() << "\r\n";
        delete resp;
    }
}

#endif /* GLOBALS_H_ */
