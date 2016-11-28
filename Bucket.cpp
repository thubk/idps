#include "Bucket.h"

Bucket::Bucket() :
		counter(0), distNum(0), L_bits(64, vector<bool>(14, false)) {
}
Bucket::~Bucket() {
}
void Bucket::increaseCounter() {
	this->counter += 1;
}
void Bucket::decreaseCounter() {
	this->counter -= 1;
}
uint16_t Bucket::getCounter() {
	return this->counter;
}
uint16_t Bucket::getDistNum() {
	return this->distNum;
}


