
#include "CountMinSketch.h"

CountMinSketch::CountMinSketch(uint64_t m_bits, uint8_t numHashes,
		Bucket bucket) :
		k_numHashes(numHashes), m_bits_row(numHashes,
				vector<Bucket>(m_bits, Bucket())), bf_row(numHashes,
				vector<BloomFilter>(m_bits, BloomFilter())) {

}

/* BCS ALGORITHM */
void CountMinSketch::process(const uint8_t *key1, const uint8_t *key2, size_t lenKey) {
	auto hashValues = hashFunctionBCS(key1, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		m_bits_row[n][index].increaseCounter();
	}

}

bool CountMinSketch::contain(const uint8_t *key, size_t lenKey) {
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

uint16_t CountMinSketch::getMinFrequence(const uint8_t *key, size_t lenKey) {
	auto hashValues = hashFunctionBCS(key, lenKey);
	uint16_t index0 = nthHashFunctionBCS(0, hashValues[0], hashValues[1],
			m_bits_row[0].size());
	uint16_t _min = m_bits_row[0][index0].getCounter();
	for (int n = 0; n < k_numHashes; n++) {
		uint16_t index = nthHashFunctionBCS(n, hashValues[0], hashValues[1],
				m_bits_row[n].size());
		uint16_t temp = m_bits_row[n][index].getCounter();
		_min = (_min > temp) ? temp : _min;
	}
	return _min;
}

