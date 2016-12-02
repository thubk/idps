#include "protocols.h"

bool readStatus(char* msg){
	bool status = false;
	string s = (string) msg;
	s = s.substr(strlen(START));
	s = s.substr(strlen(OPEN_STATUS));
	size_t f = s.find(CLOSE_STATUS);
	string v = s.substr(0, f);
	f = s.find(END);
	s = s.substr(f);
	if (s == END) {
		if(v == OK){
			status = true;
		}
	}
	return status;
}

std::array<uint32_t, 4> readMessage(char* msg) {
	std::array<uint32_t, 4> values{};
	string s = (string) msg;
	s = s.substr(strlen(START));
	s = s.substr(strlen(OPEN_FREQUENCY));
	size_t f = s.find(CLOSE_FREQUENCY);
	string v1 = s.substr(0, f);
	f = s.find(OPEN_ASYMMETRIC);
	s = s.substr(f);
	s = s.substr(strlen(OPEN_ASYMMETRIC));
	f = s.find(CLOSE_ASYMMETRIC);
	string v2 = s.substr(0, f);
	f = s.find(OPEN_DISTNUM);
	s = s.substr(f);
	s = s.substr(strlen(OPEN_DISTNUM));
	f = s.find(CLOSE_DISTNUM);
	string v3 = s.substr(0, f);
	f = s.find(END);
	s = s.substr(f);
	if (s == END) {
		/* send status : OK */
		values[0] = atoi(v1.c_str());
		values[1] = atoi(v2.c_str());
		values[2] = atoi(v3.c_str());
		values[3] = 1;
	} else {
		/* send status: NO */
	}
	return values;
}
char* sendSTATUS(string status) {
	string s = "<msg><status>" + status + "</status></msg>";
	char* msg = new char[s.length()];
	strcpy(msg, s.c_str());
	return msg;
}

char* sendMessage(uint32_t freq, uint32_t asym, uint32_t distnum) {
	ostringstream v1;
	ostringstream v2;
	ostringstream v3;
	v1 << freq;
	v2 << asym;
	v3 << distnum;
	string s = "<msg><freq>" + v1.str() + "</freq><asym>" + v2.str()
			+ "</asym><dist>" + v3.str() + "</dist></msg>";
	char* msg = new char[s.length()];
	strcpy(msg, s.c_str());
	return msg;
}
