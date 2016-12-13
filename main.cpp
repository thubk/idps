#include <iostream>
#include <fstream>
#include <math.h>
#include <pthread.h>
#include "pfringcpp.cpp"
#include "master.cpp"
#include "slave.cpp"
#include "globals.h"
#include "protocols.h"


using namespace std;

#define DEFAULT_ADDRESS "192.168.100.162" /* simulator interface */
#define TIMEINTERVAL_START 20
#define MAX_THREAD 9

/* thread for phase 1 -> worker: master */
#define DELTA_TIME1 5
#define DELTA_TIME2 5
void * handlePhase1(void *) {
	puts("Phase 1: running...");
	float alpha = 0.4;
	float theta = 0.5;
	uint32_t server_key = getIPAddress(server_list[master_index].c_str());
	while (1) {
		sleep(DELTA_TIME1);
		double counter = phase1.getMinCounter(&server_key, sizeof(uint32_t));
		if (!master.getFlag()) {
			double prior_counter = master.getPriorCounter();
			master.setV(counter - prior_counter);
			double v = master.getV();
			double ex_v_delta = master.getExVDelta();
			double ex_v = (1 - alpha) * ex_v_delta + alpha * v;
			if ((v > ((1 + theta) * ex_v_delta))) {
				master.setFlag(true);
				phase_flag = true;
				master.setVBackup(ex_v_delta);
			}
			/* update v_delta and ex_v_delta */
			master.updatePriorCounter(counter);
			master.setVDelta(v);
			master.setExVDelta(ex_v);
			cout << "Phase 1: VDelta = " << master.getVDelta() << " ExVDelta = "
					<< master.getExVDelta() << "\n";

		} else {
			double prior_counter = master.getPriorCounter();
			master.setV(counter -prior_counter);
			double v = master.getV();
			double v_backup = master.getVBackup();
			master.setExV((1 - alpha) * v_backup + alpha * v);

			double ex_v = master.getExV();

			if ((v < ((1 + theta) * v_backup))) {
				master.setFlag(false);
				phase_flag = false;
			}
			/* update v_delta and ex_v_delta */
			master.updatePriorCounter(counter);
			master.setVDelta(v);
			master.setExVDelta(ex_v);
			cout << "Phase 1: VDelta = " << master.getVDelta() << " ExVDelta = "
					<< master.getExVDelta() << "\n";
		}
	}
	return NULL;
}

/* thread for phase 2,3 -> worker: master */
void * handlePhase23(void *) {
	uint32_t threshold = 500; /* modify  */
	puts("Phase 2: running...");
	int beta = 2;
	uint32_t server_key = getIPAddress(server_list[master_index].c_str());
	while (1) {
		if (phase_flag) {
			sleep(DELTA_TIME2);
			double counter = (double) phase2.getMinFrequence(&server_key,
					sizeof(uint32_t));
			double sum_asym = (double) sumList(exasym_num);
			double c = sum_asym / MAX_MASTER;/* review */
			double d = pow((counter - c), 2);
			if (((counter - c) >= (beta * d))) {/* check distNum*/
				uint32_t sum_distNum = sumList(exdist_num);
				uint32_t est_distNum = phase2.getDistNumBCS(&server_key,
						sizeof(uint32_t), server_key);
				cout << "Phase 2: (Counter - C) = " << (counter - c)
						<< " DistNum = " << (sum_distNum + est_distNum) << "\n";
				if ((sum_distNum + est_distNum) > threshold) {
					attack[master_index] = true; /* attack */
				}
			} else {
				attack[master_index] = false; /* normal */
			}
		}
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t thread[MAX_THREAD];
	string len;
	string line;
	ifstream infile("config");
	getline(infile, len);
	length = atoi(len.c_str());
	int i = 0;
	while (getline(infile, line)) {
		string addr = line;
		size_t f = addr.find(":");
		worker_list[i] = addr.substr(0, f);
		addr = addr.substr(f + 1);
		f = addr.find(":");
		server_list[i] = addr.substr(0, f);
		i++;
	}
	pthread_create(&thread[length], NULL, startPFring, NULL);
	sleep(TIMEINTERVAL_START);
	/* start Master */
	for (int n = 0; n < length; n++) {
		if (worker_list[n] == DEFAULT_ADDRESS) {/* note: change default */
			master_index = n;
			used_address[n] = true; /* using */
			pthread_create(&thread[n], NULL, processSocketServer, NULL);
		}
	}
	/* start Slaves */
	sleep(TIMEINTERVAL_START);
	for (int n = 0; n < length; n++) {
		if (worker_list[n] != DEFAULT_ADDRESS) {
			pthread_create(&thread[n], NULL, processSocketClient, NULL);
		}
	}
	sleep(TIMEINTERVAL_START);
	pthread_create(&thread[length + 1], NULL, handlePhase1, NULL);/* note: ID */
	pthread_create(&thread[length + 2], NULL, handlePhase23, NULL);
	while (1) {
	};
	return 0;
}
