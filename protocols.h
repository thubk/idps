
#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include <array>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#define OPEN_FREQUENCY 		"<freq>"
#define CLOSE_FREQUENCY 	"</freq>"
#define OPEN_ASYMMETRIC		"<asym>"
#define CLOSE_ASYMMETRIC 	"</asym>"
#define OPEN_DISTNUM 		"<dist>"
#define CLOSE_DISTNUM 		"</dist>"
#define OPEN_STATUS			"<status>"
#define CLOSE_STATUS		"</status>"
#define START				"<msg>"
#define END					"</msg>"
#define OK	"OK"
#define NO 	"NO"
using namespace std;

char* sendSTATUS(string status);
char* sendMessage(uint32_t freq, uint32_t asym, uint32_t distnum);
bool readStatus(char *msg);
std::array<uint32_t, 4> readMessage(char* msg);
#endif /* PROTOCOLS_H_ */
