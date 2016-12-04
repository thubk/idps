#include "protocols.h"

std::array<uint32_t, 5> readMessage(char* msg) {
	std::array<uint32_t, 5> values { 0 };
	string s = (string) msg;
	s = s.substr(strlen(START));
	s = s.substr(strlen(OPEN_CODE));
	size_t f = s.find(CLOSE_CODE);
	string code = s.substr(0, f);
	f = s.find(OPEN_VALUE);
	s = s.substr(f);
	s = s.substr(strlen(OPEN_VALUE));
	f = s.find(CLOSE_VALUE);
	string value = s.substr(0, f);
	f = s.find(END);
	s = s.substr(f);
	if (s == END) {
		/* send status : OK */
		size_t f1 = value.find(":");
		string v1 = value.substr(0, f1);
		value = value.substr(f1 + 1);
		f1 = value.find(":");
		string v2 = value.substr(0, f1);
		value = value.substr(f1 + 1);
		f1 = value.find(":");
		string v3 = value.substr(0, f1);
		string v4 = value.substr(f1 + 1);
		values[0] = atoi(code.c_str());
		values[1] = atoi(v1.c_str());
		values[2] = atoi(v2.c_str());
		values[3] = atoi(v3.c_str());
		values[4] = atoi(v4.c_str());
	} else {
		/* send status: NO */
	}
	return values;
}

/* code: 0:request 1,2,3:phase, 3:status */
char* sendMessage(int code, uint32_t num1, uint32_t num2, uint32_t num3,
		uint32_t num4) {
	ostringstream c;
	ostringstream v1;
	ostringstream v2;
	ostringstream v3;
	ostringstream v4;
	c << code;
	v1 << num1;/* number of packets *//* phase 1,2*/
	v2 << num2;/* asymmetric *//* active -> attack */
	v3 << num3;/* distinguish number *//* num2: active -> num3 server index */
	v4 << num4;/* sum of a row hash */
	string s = "<msg><code>" + c.str() + "</code><value>" + v1.str() + ":"
			+ v2.str() + ":" + v3.str() + ":" + v4.str() + "</value></msg>";
	char* msg = new char[s.length()];
	strcpy(msg, s.c_str());
	return msg;
}
