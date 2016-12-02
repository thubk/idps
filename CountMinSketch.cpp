#include "CountMinSketch.h"

void CountMinSketch::process(const void * key1, size_t lenKey, uint32_t key) {
	bool flag = false;
	auto hashValues = hashFunctionCMS(key1, lenKey);
	if (contain(key1, lenKey, key)) {
		flag = true;
	}
	for (uint8_t n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionCMS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (!flag) {
			if (m_bits_row[n][index].getOwner() == 0) {
				m_bits_row[n][index].setOwner(key);
				m_bits_row[n][index].increaseCounter();
				break;
			}
		} else {
			if (m_bits_row[n][index].getOwner() == key) {
				m_bits_row[n][index].increaseCounter();
				break;
			}
		}
	}
}

bool CountMinSketch::contain(const void * key1, size_t lenKey, uint32_t key) {
	auto hashValues = hashFunctionCMS(key1, lenKey);
	for (uint8_t n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionCMS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (m_bits_row[n][index].getOwner() == key) {
			return true;
		}
	}
	return false;
}

