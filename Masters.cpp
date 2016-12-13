#include "Masters.h"

void Masters::updatePriorCounter(double num) {
	this->prior_counter = num;
}

void Masters::setV(double v) {
	this->v = v;
}
void Masters::setVDelta(double v_delta){
	this->v_delta = v_delta;
}
void Masters::setVBackup(double v_backup) {
	this->v_backup = v_backup;
}
void Masters::setExV(double ex_v){
	this->ex_v = ex_v;
}
void Masters::setExVDelta(double ex_v_delta){
	this->ex_v_delta = ex_v_delta;
}
void Masters::setOwner(double key){
	owner = key;
}
void Masters::setFlag(bool flag) {
	this->flag = flag;
}
double Masters::getOwner() {
	return owner;
}
double Masters::getV() {
	return v;
}
double Masters::getVDelta(){
	return v_delta;
}
double Masters::getVBackup(){
	return v_backup;
}
double Masters::getExV(){
	return ex_v;
}
double Masters::getExVDelta(){
	return ex_v_delta;
}
bool Masters::getFlag(){
	return flag;
}

double Masters::getPriorCounter() {
	return this->prior_counter;
}
