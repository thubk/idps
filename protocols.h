
#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include <array>
#include <cstring>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#define OPEN_VALUE			"<value>"
#define CLOSE_VALUE			"</value>"
#define START				"<msg>"
#define END					"</msg>"
#define OPEN_CODE			"<code>"
#define CLOSE_CODE			"</code>"
#define OK	"OK"
#define NO 	"NO"
using namespace std;

/* num1: counter phase 1, num2: counter phase2, num3: distinguish num */
char* sendMessage(uint8_t code, uint32_t num1, uint32_t num2, uint32_t num3);
std::array<uint32_t, 4> readMessage(char* msg);
#endif /* PROTOCOLS_H_ */
