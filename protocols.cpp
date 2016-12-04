#include "protocols.h"

std::array<uint32_t, 4> readMessage(char* msg) {
	std::array<uint32_t, 4> values {0};
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
		if (atoi(code.c_str()) == 2) {
			size_t f1 = value.find(":");
			string v1 = value.substr(0, f1);
			value = value.substr(f1+1);
			f1 = value.find(":");
			string v2 = value.substr(0, f1);
			string v3 = value.substr(f1+1); /* note: '\0' */
			values[0] = atoi(code.c_str());
			values[1] = atoi(v1.c_str());
			values[2] = atoi(v2.c_str());
			values[3] = atoi(v3.c_str());
		} else {
			values[0] = atoi(code.c_str());
			values[1] = atoi(value.c_str());
		}

	} else {
		/* send status: NO */
	}
	return values;
}

/* code: 0:active, 1,2,3:phase, 3:status */
char* sendMessage(uint8_t code, uint32_t num1, uint32_t num2, uint32_t num3) {
	ostringstream c;
	ostringstream v1;
	ostringstream v2;
	ostringstream v3;
	c << unsigned(code);
	v1 << num1;
	v2 << num2;
	v3 << num3;
	string s = "<msg><code>" + c.str() + "</code><value>" + v1.str() + ":"
			+ v2.str() + ":" + v3.str() + "</value></msg>";
	char* msg = new char[s.length()];
	strcpy(msg, s.c_str());
	return msg;
}
