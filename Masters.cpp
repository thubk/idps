#include "Masters.h"

void Masters::increaseCounter(){
	counter++;
}
void Masters::setV(uint32_t v) {
	this->v = v;
}
void Masters::setVDelta(uint32_t v_delta){
	this->v_delta = v_delta;
}
void Masters::setVBackup(uint32_t v_backup) {
	this->v_backup = v_backup;
}
void Masters::setOwner(uint32_t key){
	owner = key;
}
void Masters::setFlag(bool flag) {
	this->flag = flag;
}
uint32_t Masters::getOwner() {
	return owner;
}
uint32_t Masters::getV() {
	return v;
}
uint32_t Masters::getVDelta(){
	return v_delta;
}
uint32_t Masters::getVBackup(){
	return v_backup;
}

