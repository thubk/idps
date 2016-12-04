/*
 * BucketCMS.h
 *
 *  Created on: Dec 2, 2016
 *      Author: root
 */

#ifndef MASTERS_H_
#define MASTERS_H_
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "MurmurHash3.h"
using namespace std;

class Masters{
private:
	uint32_t owner;
	uint32_t counter;
	uint32_t v;
	uint32_t v_delta;
	uint32_t v_backup;
	bool flag;
public:
	Masters(): owner(0), counter(0), v(0), v_delta(0), v_backup(0), flag(false){}
	~Masters(){}
	void increaseCounter();
	void setV(uint32_t v);
	void setVDelta(uint32_t v_delta);
	void setVBackup(uint32_t v_backup);
	void setOwner(uint32_t key);
	void setFlag(bool flag);
	uint32_t getOwner();
	uint32_t getV();
	uint32_t getVDelta();
	uint32_t getVBackup();
};



#endif /* MASTERS_H_ */
