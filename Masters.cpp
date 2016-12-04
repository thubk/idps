#include "Masters.h"

void Masters::updateCounter(uint32_t num){
	counter = num;
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
void Masters::setExV(uint32_t ex_v){
	this->ex_v = ex_v;
}
void Masters::setExVDelta(uint32_t ex_v_delta){
	this->ex_v_delta = ex_v_delta;
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
uint32_t Masters::getExV(){
	return ex_v;
}
uint32_t Masters::getExVDelta(){
	return ex_v_delta;
}
bool Masters::getFlag(){
	return flag;
}
uint32_t Masters::getCounter(){
	return counter;
}
