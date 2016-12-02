#include "BucketCMS.h"

void BucketCMS::increaseCounter(){
	counter++;
}
void BucketCMS::setV(uint32_t v) {
	this->v = v;
}
void BucketCMS::setVDelta(uint32_t v_delta){
	this->v_delta = v_delta;
}
void BucketCMS::setVBackup(uint32_t v_backup) {
	this->v_backup = v_backup;
}
void BucketCMS::setOwner(uint32_t key){
	owner = key;
}
void BucketCMS::setFlag(bool flag) {
	this->flag = flag;
}
uint32_t BucketCMS::getOwner() {
	return owner;
}
uint32_t BucketCMS::getV() {
	return v;
}
uint32_t BucketCMS::getVDelta(){
	return v_delta;
}
uint32_t BucketCMS::getVBackup(){
	return v_backup;
}

