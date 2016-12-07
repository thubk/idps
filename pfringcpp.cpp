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
#include <signal.h>
#include "pfring.h"
#include "pcap.h"
#include "globals.h"
using namespace std;

#define DEFAULT_SNAPLEN 	128
#define MAX_NUM_THREADS 	64
#define DEFAULT_DEVICE     "wlan0"
#define NO_ZC_BUFFER_LEN	9000
#define DELTA_TIME3 5

#define THRESHOLD 80 /* 80 SYN packets */
pfring *pd;
int num_threads = 2;

u_int8_t wait_for_packet = 1;
u_int8_t use_extended_pkt_header = 0;

uint64_t name = 0;


bool checkServerIP(uint32_t ip) {/* check IP fall in server_list */
	char* addr = intoa(ip);
	for (int i = 0; i < MAX_MASTER; i++) {
		if (server_list[i] == addr) {/* review */
			return true;
		}
	}
	return false;
}

int getAttackVictim() {
	int index = -1; /* no victim */
	for (int i = 0; i < MAX_MASTER; i++) {
		if (attack[i]) {
			index = i;
		}
	}
	return index;
}

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

void processsPacket(const struct pfring_pkthdr *hdr, const u_char *p,
		const u_char *user_bytes) {
	long threadId = (long) user_bytes;
	uint32_t src_key = hdr->extended_hdr.parsed_pkt.ip_src.v4;
	uint32_t dst_key = hdr->extended_hdr.parsed_pkt.ip_dst.v4;
	/* phase 1 */
	if (checkServerIP(dst_key)) {
		phase1.process(&dst_key, sizeof(uint32_t));
	}
	/* phase 2 */
	if (phase_flag) {/* phase 2: active */
		if (checkServerIP(dst_key)) {/* --> dst */
			uint64_t key = (uint64_t(dst_key) << 32) + uint64_t(src_key);
			phase2.process(&dst_key, &key, &src_key, sizeof(uint32_t), dst_key);
			/* SYN flag *//* review RST */
			uint8_t flag = hdr->extended_hdr.parsed_pkt.tcp.flags;
			if (flag == 2) { /* SYN flag -> src*/
				syn_list.process(&src_key, sizeof(uint32_t));
				if (syn_list.getMinCounter(&src_key, sizeof(uint32_t)) >= THRESHOLD) {
					exsyn_list.add(&src_key, sizeof(uint32_t));
				}
			}
		}
		if (checkServerIP(src_key)) {/* src --> */
			uint64_t key = (uint64_t(src_key) << 32) + uint64_t(dst_key);
			phase2.update(&src_key, &key, sizeof(uint32_t));
		}
	}
	/* attack -> process src */
	if (getAttackVictim() > 0) {
		int index = getAttackVictim();
		if (server_list[index] == intoa(dst_key)) {
			if (exsyn_list.contain(&src_key, sizeof(uint32_t))) {
				/* send a rule drop -> controller */
				if(!drop_list.contain(&src_key, sizeof(uint32_t))) {
					postrules(src_key,name);
					drop_list.add(&src_key, sizeof(uint32_t));
					name++;

				}

			}
		}
	}

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
		device = (char *) DEFAULT_DEVICE;

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

