#include "Bucket.h"

void Bucket::increaseCounter() {
	counter += 1;
}
void Bucket::decreaseCounter() {
	counter -= 1;
}
void Bucket::setOwner(uint32_t key) {
	owner = key;
}
uint32_t Bucket::getCounter() {
	return counter;
}
uint32_t Bucket::getDistNum() {
	return distNum;
}

uint32_t Bucket::getOwner() {
	return owner;
}

void Bucket::estimatePCSA64(const void * key, size_t lenKey) {
	auto hashValues = hashFunctionBucket(key, lenKey);/* 192.0.0.0/16 */
	uint64_t value = nthHashFunctionBucket(10, hashValues[0], hashValues[1]);
	uint16_t x = value % k_numHashes;
	uint16_t b = getFirstIndex64(value, k_numHashes);
	L_bits[x][b] = true;
	uint32_t z = 0;
	for (int n = 0; n < k_numHashes; n++) {
		for (int i = 0; i < m_bits; i++) {
			if (!L_bits[n][i]) {
				z += i;
				break;
			}
		}
	}
	distNum = (k_numHashes * pow(2, ((double) z / (double) k_numHashes)))
			/ 0.77351;
	cout << distNum << "\n";
}

void Bucket::estimatePCSA32(uint32_t key) {
	uint32_t value = 2639256300 * key + 3343646820;
	uint16_t x = value % k_numHashes;
	uint16_t b = getFirstIndex32(value, k_numHashes);
	L_bits[x][b] = true;
	uint32_t z = 0;
	for (int n = 0; n < k_numHashes; n++) {
		for (int i = 0; i < m_bits; i++) {
			if (!L_bits[n][i]) {
				z += i;
				break;
			}
		}
	}
	distNum = (k_numHashes * pow(2, ((double) z / (double) k_numHashes)))
			/ 0.77351;
}

/* HLL */
void Bucket::estimateHLL64(const void * key, size_t lenKey) {
	/* initialization */
	auto hashValues = hashFunctionBucket(key, lenKey);/* 192.0.0.0/16 */
	uint64_t value = nthHashFunctionBucket(0, hashValues[0], hashValues[1]);
	uint16_t x = value % k_numHashes;
	uint16_t b = getFirstIndex64(value, k_numHashes);
	M[x] = M[x] < b ? b : M[x];
	uint32_t z = 0;
	for (int n = 0; n < k_numHashes; n++) {
		z += M[n];
	}
	distNum = (0.79402 * k_numHashes - 0.84249)
			* pow(2, (double) z / (double) k_numHashes);
}

void Bucket::estimateHLL32(uint32_t key) {
	/* h(v) = a*v + b ; a,b uniformly and independently */
	uint32_t value = 2639256300 * key + 3343646820;
	uint16_t x = value % k_numHashes;
	uint16_t b = getFirstIndex32(value, k_numHashes);
	M[x] = M[x] < b ? b : M[x];
	uint32_t z = 0;
	for (int n = 0; n < k_numHashes; n++) {
		z += M[n];
	}
	distNum = (0.79402 * k_numHashes - 0.84249)
			* pow(2, (double) z / (double) k_numHashes);
}
