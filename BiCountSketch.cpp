#include "BiCountSketch.h"

/* BCS ALGORITHM */

/* key1: DIP , key2: DIP|SIP, key3: SIP, key: DIP(uint32) */
void BiCountSketch::process(const void * key1, const void * key2,
		const void * key3, size_t lenKey, uint32_t key) {

	bool flag = false;/* first */
	auto hashValues = hashFunctionBCS(key1, lenKey);
	if (contain(key1, lenKey)) {
		flag = true;
	}
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (m_bits_row[n][index].getCounter() == 0) {
			bf_row[n].add(key2, sizeof(uint64_t)); //review 32 - 64
			m_bits_row[n][index].increaseCounter();
		} else {
			if (!bf_row[n].contain(key2, sizeof(uint64_t))) { //review
				bf_row[n].add(key2, sizeof(uint64_t)); //review
				m_bits_row[n][index].increaseCounter();
			}
		}
	}
	int k = getHashIndex(key1, lenKey, flag, key);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (n == k) {
			m_bits_row[n][index].estimatePCSA64(key3, sizeof(uint32_t));
			//m_bits_row[n][index].estimateHLL64(key3, sizeof(uint32_t));
			m_bits_row[n][index].setOwner(key);
			break;
		}
	}
}
/* review */
void BiCountSketch::update(const void * key1, const void * key2,
		size_t lenKey) {
	auto hashValues = hashFunctionBCS(key1, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (m_bits_row[n][index].getCounter() != 0) {
			if (bf_row[n].contain(key2, sizeof(uint64_t))) { //review
				m_bits_row[n][index].decreaseCounter();
			}
		}
	}
}

bool BiCountSketch::contain(const void * key, size_t lenKey) {
	auto hashValues = hashFunctionBCS(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (m_bits_row[n][index].getCounter() == 0) {
			return false;
		}
	}
	return true;
}

uint32_t BiCountSketch::getMinFrequence(const void * key, size_t lenKey) {
	auto hashValues = hashFunctionBCS(key, lenKey);
	uint16_t index0 = nthHashFunctionBCS(0, hashValues[0], hashValues[1],
			m_bits_row[0].size());
	uint32_t _min = m_bits_row[0][index0].getCounter();
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		uint32_t temp = m_bits_row[n][index].getCounter();
		_min = (_min > temp) ? temp : _min;
	}
	return _min;
}

int BiCountSketch::getHashIndex(const void * key1, size_t lenKey, bool flag,
		uint32_t key) {
	auto hashValues = hashFunctionBCS(key1, lenKey);
	int temp = -1;
	if (!flag) {
		for (int n = 0; n < k_numHashes; n++) {
			uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
					m_bits_row[n].size());
			if (m_bits_row[n][index].getCounter() == 1) {
				temp = n;
				break;
			}
		}
	} else {
		for (int n = 0; n < k_numHashes; n++) {
			uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
					m_bits_row[n].size());
			if (m_bits_row[n][index].getOwner() == key) {/* review */
				temp = n;
				break;
			}
		}
	}
	return temp;
}

uint32_t BiCountSketch::getDistNumBCS(const void * key1, size_t lenKey,
		uint32_t key) {
	uint32_t temp = 0;
	auto hashValues = hashFunctionBCS(key1, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (m_bits_row[n][index].getOwner() == key) {
			temp = m_bits_row[n][index].getDistNum();
			break;
		}
	}
	return temp;
}

int BiCountSketch::getHashIndexBucket(const void * key1, size_t lenKey,
		uint32_t key) {
	int temp = -1;
	auto hashValues = hashFunctionBCS(key1, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1], m_bits_row[n].size());
		if (m_bits_row[n][index].getOwner() == key) {
			temp = n;
			break;
		}
	}
	return temp;
}

uint32_t BiCountSketch::getSumHashesRow(int index) {
	uint32_t temp = 0;
	for (int n = 0; n < m_bits_row.size(); n++) {
		temp += m_bits_row[index][n].getCounter();
	}
	return temp;
}

