#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <time.h>
#include <errno.h>
#include "pfring.h"
#include "pcap.h"

using namespace std;

#define ALARM_SLEEP			1
#define DEFAULT_SNAPLEN 	128
#define MAX_NUM_THREADS 	64
#define DEFAULT_DEVICE     "wlan0"
#define NO_ZC_BUFFER_LEN	9000

pfring *pd;
int verbose = 0, num_threads = 1;
pfring_stat pfringStats;

static struct timeval startTime;


char* intoa(unsigned int addr);


u_int8_t wait_for_packet = 1;
u_int8_t use_extended_pkt_header = 0;


int bind2core(u_int core_id) {
	cpu_set_t cpuset;
	int s;

	CPU_ZERO(&cpuset);
	CPU_SET(core_id, &cpuset);
	if ((s = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset))
			!= 0) {
		cout << "Error while binding to core!!!" << endl;
		return (-1);
	} else {
		return (0);
	}
}
void processsPacket(const struct pfring_pkthdr *h, const u_char *p,
		const u_char *user_bytes) {
	long threadId = (long) user_bytes;
	// code here

}
void* packet_consumer_thread(void* _id) {
	long thread_id = (long) _id;
	u_int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
	u_char buffer[NO_ZC_BUFFER_LEN];
	u_char *buffer_p = buffer;

	u_long core_id = thread_id % numCPU;
	struct pfring_pkthdr hdr;
	if (num_threads > 1 && numCPU > 1) {
		if (bind2core(core_id) == 0) {
			cout << "Set thread " << thread_id << " on core " << core_id <<  "/" << numCPU << endl;
		}
	}
	memset(&hdr, 0, sizeof(hdr));
	while (1) {
		int rc;
		u_int len;
		if ((rc = pfring_recv(pd, &buffer_p, NO_ZC_BUFFER_LEN, &hdr,
				wait_for_packet)) > 0) {
			processsPacket(&hdr, buffer, (u_char*) thread_id);
			cout << intoa(hdr.extended_hdr.parsed_pkt.ip_src.v4) << endl;
			cout << unsigned(hdr.extended_hdr.parsed_pkt.tcp.flags) << endl;


		} else {
			if (wait_for_packet == 0) sched_yield();
		}
	}
	return NULL;
}
int startPFring() {
	char *device = NULL, c, buff[32], path[256] = { 0 }, *reflector_device =
	NULL;
	int promisc = 1, snaplen = DEFAULT_SNAPLEN, rc;
	u_int32_t flags = 0;
	int bind_core = -1;
	packet_direction direction = rx_and_tx_direction; //review

	/* review */
	num_threads = 5;
	use_extended_pkt_header = 1;

	if (device == NULL) device = DEFAULT_DEVICE;

	bind2core(bind_core);

	if (num_threads > 1) 			flags |= PF_RING_REENTRANT;
	if (use_extended_pkt_header)	flags |= PF_RING_LONG_HEADER;
	if (promisc) 					flags |= PF_RING_PROMISC;
									flags |= PF_RING_ZC_SYMMETRIC_RSS;

	pd = pfring_open(device, snaplen, flags);
	pfring_enable_ring(pd);
	if (pd == NULL) {
		cout << "pfring_open error [%s] (pf_ring not loaded or interface %s is down ?)\n" << endl;
		return -1;
	}
	cout << "# Device RX channels: " << unsigned(pfring_get_num_rx_channels(pd)) << endl;
	cout << "# Polling threads: " << num_threads << endl;
	pfring_set_direction(pd, direction);
	if (num_threads <= 1) {

	} else {
		pthread_t my_thread;
		long i;
		for (i = 0; i < num_threads; i++)
			pthread_create(&my_thread, NULL, packet_consumer_thread, (void*) i);

		for (i = 0; i < num_threads; i++)
			pthread_join(my_thread, NULL);
	}
	return 0;
}

char* _intoa(unsigned int addr, char* buf, u_short bufLen) {
	char *cp, *retStr;
	u_int byte;;
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
			if(byte > 0)
				*--cp = byte + '0';
		}
		*--cp = '.';
		addr >>= 8;
	} while (--n > 0);
	retStr = (char*)(cp+1);
	return retStr;
}

char* intoa(unsigned int addr) {
	static char buf[sizeof "fff.fff.fff.fff"];
	return _intoa(addr, buf, sizeof(buf));
}

