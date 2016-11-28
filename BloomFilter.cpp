/*
 * BloomFilter.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: root
 */
#include "BloomFilter.h"

BloomFilter::BloomFilter() :
		m_bits(65536, false), k_numHashes(32) {
}
BloomFilter::BloomFilter(uint64_t m_bits, uint8_t numHashes) :
		m_bits(m_bits, false), k_numHashes(numHashes) {
}

void BloomFilter::add(const uint8_t *key, size_t lenKey) {
	auto hashValues = hashFunctionBF(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		m_bits[nthHashFunctionBF(n, hashValues[0], hashValues[1], m_bits.size())] = true;
	}
}


bool BloomFilter::contain(const uint8_t *key, size_t lenKey){
	auto hashValues = hashFunctionBF(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		if (!m_bits[nthHashFunctionBF(n, hashValues[0], hashValues[1], m_bits.size())]) {
			return false;
		}
	}
	return true;;
}

