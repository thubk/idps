#include "CountMinSketch.h"

void CountMinSketch::process(const void * key, size_t lenKey) {
	bool flag = false;
	auto hashValues = hashFunctionCMS(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionCMS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		m_bits_row[n][index] += 1;
	}
}

bool CountMinSketch::contain(const void * key, size_t lenKey) {
	auto hashValues = hashFunctionCMS(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionCMS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		if (m_bits_row[n][index] == 0) {
			return false;
		}
	}
	return true;
}
uint32_t CountMinSketch::getMinCounter(const void * key, size_t lenKey) {
	auto hashValues = hashFunctionCMS(key, lenKey);
	uint32_t index0 = nthHashFunctionCMS(0, hashValues[0], hashValues[1],
			m_bits_row[0].size());
	uint32_t _min = m_bits_row[0][index0];
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionCMS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		uint32_t temp = m_bits_row[n][index];
		_min = (_min > temp) ? temp : _min;
	}
	return _min;
}

/* reset freq after send */
void CountMinSketch::resetCounter(const void * key, size_t lenKey, uint32_t min) {
	auto hashValues = hashFunctionCMS(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionCMS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		m_bits_row[n][index] -= min;
	}
}
/* update freq once receive */
void CountMinSketch::updateCounter(const void * key, size_t lenKey, uint32_t min) {
	auto hashValues = hashFunctionCMS(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionCMS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		m_bits_row[n][index] += min;
	}
}


