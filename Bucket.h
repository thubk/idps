#ifndef BUCKET_H_
#define BUCKET_H_
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
using namespace std;

class Bucket {
private:
	uint16_t counter;
	uint16_t distNum;
	std::vector< std::vector<bool> > L_bits;
public:
	Bucket();
	~Bucket();
	void increaseCounter();/* +1 */
	void decreaseCounter();/* -1 */
	uint16_t getCounter();
	uint16_t getDistNum();

};




#endif /* BUCKET_H_ */
