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
#include "globals.h"
using namespace std;

#define DEFAULT_SNAPLEN 	128
#define MAX_NUM_THREADS 	64
#define DEFAULT_DEVICE     "wlan0"
#define NO_ZC_BUFFER_LEN	9000


pfring *pd;
int num_threads = 3;

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

double delta_time(struct timeval * now, struct timeval * before) {
	time_t delta_seconds;
	time_t delta_microseconds;

	delta_seconds = now->tv_sec - before->tv_sec;
	delta_microseconds = now->tv_usec - before->tv_usec;

	if (delta_microseconds < 0) {
		delta_microseconds += 1000000;
		--delta_seconds;
	}
	return ((double) (delta_seconds * 1000) + (double) delta_microseconds / 1000);
}



void processsPacket(const struct pfring_pkthdr *hdr, const u_char *p,
		const u_char *user_bytes) {
	long threadId = (long) user_bytes;
	cout << "Thread Id: " << threadId << "\n";
	//string ip_src = intoa(hdr->extended_hdr.parsed_pkt.ip_src.v4);
	//cout << unsigned(hdr->extended_hdr.parsed_pkt.tcp.flags) << "\n";
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
			cout << "Set Thread " << thread_id << " on core " << core_id << "/"
					<< numCPU << endl;
		}
	}
	memset(&hdr, 0, sizeof(hdr));
	while (1) {/* running */
		int rc;
		u_int len;
		if ((rc = pfring_recv(pd, &buffer_p, NO_ZC_BUFFER_LEN, &hdr,
				wait_for_packet)) > 0) {
			processsPacket(&hdr, buffer, (u_char*) thread_id);
		} else {
			if (wait_for_packet == 0)
				sched_yield();
		}
	}
	return NULL;
}
void * startPFring(void *) {
	char *device = NULL, c, buff[32], path[256] = { 0 }, *reflector_device =
	NULL;
	int promisc = 1, snaplen = DEFAULT_SNAPLEN, rc;
	u_int32_t flags = 0;
	int bind_core = -1;
	packet_direction direction = rx_and_tx_direction; //review

	/* review */
	use_extended_pkt_header = 1;

	if (device == NULL)
		device = (char *)DEFAULT_DEVICE;

	bind2core(bind_core);

	if (num_threads > 1)
		flags |= PF_RING_REENTRANT;
	if (use_extended_pkt_header)
		flags |= PF_RING_LONG_HEADER;
	if (promisc)
		flags |= PF_RING_PROMISC;
	flags |= PF_RING_ZC_SYMMETRIC_RSS;

	pd = pfring_open(device, snaplen, flags);
	pfring_enable_ring(pd);
	if (pd == NULL) {
		cout
				<< "pfring_open error [%s] (pf_ring not loaded or interface %s is down ?)\n"
				<< endl;
		return NULL;
	}
	cout << "# Device RX channels: " << unsigned(pfring_get_num_rx_channels(pd))
			<< endl;
	cout << "# Polling threads: " << num_threads << endl;

	pfring_set_direction(pd, direction);

	if (num_threads <= 1) {

	} else {
		pthread_t my_thread;
		long i;
		for (i = 0; i < num_threads; i++) {
			pthread_create(&my_thread, NULL, packet_consumer_thread, (void*) i);
		}

		for (i = 0; i < num_threads; i++) {
			pthread_join(my_thread, NULL);
		}

	}
	return NULL;
}
