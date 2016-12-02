/*
 * BloomFilter.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: root
 */
#include "BloomFilter.h"


void BloomFilter::add(const void * key, size_t lenKey) {
	auto hashValues = hashFunctionBF(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		m_bits[nthHashFunctionBF(n, hashValues[0], hashValues[1], m_bits.size())] = true;
	}
}


bool BloomFilter::contain(const void * key, size_t lenKey){
	auto hashValues = hashFunctionBF(key, lenKey);
	for (int n = 0; n < k_numHashes; n++) {
		if (!m_bits[nthHashFunctionBF(n, hashValues[0], hashValues[1], m_bits.size())]) {
			return false;
		}
	}
	return true;;
}

