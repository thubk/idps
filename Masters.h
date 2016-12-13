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
	double owner;/* worker have many masters */
	double prior_counter;
	double v;
	double v_delta;
	double v_backup;
	double ex_v;
	double ex_v_delta;
	bool flag;
public:
	Masters(): owner(0), v(0), v_delta(0), v_backup(0), flag(false), ex_v(0), ex_v_delta(0), prior_counter(0){}
	~Masters(){}
	void updatePriorCounter(double num);
	void setV(double v);
	void setVDelta(double v_delta);
	void setVBackup(double v_backup);
	void setExV(double ex_v);
	void setExVDelta(double ex_v_delta);
	void setOwner(double key);
	void setFlag(bool flag);
	double getPriorCounter();
	double getOwner();
	double getV();
	double getVDelta();
	double getVBackup();
	bool getFlag();
	double getExV();
	double getExVDelta();
};



#endif /* MASTERS_H_ */
