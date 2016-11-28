/* MurmurHash - MIT */
#ifndef BLOOMFILTER_H_
#define BLOOMFILTER_H_

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "MurmurHash3.h"
using namespace std;

class BloomFilter {
private:
	uint8_t k_numHashes;
	std::vector<bool> m_bits;
public:
	BloomFilter();
	BloomFilter(uint64_t m_bits, uint8_t numHashes);
	void add(const uint8_t *key, size_t lenKey);/* key: DIP|SIP */
	bool contain(const uint8_t *key, size_t lenKey);
};

std::array<uint64_t, 2> hashFunctionBF(const uint8_t *key, size_t lenKey) {
	 std::array<uint64_t, 2>  hashValue;
	MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
	return hashValue;
}

uint64_t nthHashFunctionBF(uint8_t nth, uint64_t hashA, uint64_t hashB, uint64_t filterSize) {
	return (hashA + nth*hashB) % filterSize;
}

#endif /* BLOOMFILTER_H_ */
