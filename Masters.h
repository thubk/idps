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
	uint32_t owner;/* worker have many masters */
	uint32_t counter;
	uint32_t v;
	uint32_t v_delta;
	uint32_t v_backup;
	uint32_t ex_v;
	uint32_t ex_v_delta;
	bool flag;
public:
	Masters(): owner(0), counter(0), v(0), v_delta(0), v_backup(0), flag(false), ex_v(0), ex_v_delta(0){}
	~Masters(){}
	void updateCounter(uint32_t num);
	void setV(uint32_t v);
	void setVDelta(uint32_t v_delta);
	void setVBackup(uint32_t v_backup);
	void setExV(uint32_t ex_v);
	void setExVDelta(uint32_t ex_v_delta);
	void setOwner(uint32_t key);
	void setFlag(bool flag);
	uint32_t getOwner();
	uint32_t getV();
	uint32_t getVDelta();
	uint32_t getVBackup();
	bool getFlag();
	uint32_t getExV();
	uint32_t getExVDelta();
	uint32_t getCounter();
};



#endif /* MASTERS_H_ */
