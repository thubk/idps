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
	int k_numHashes;
	std::vector<bool> m_bits;
public:
	BloomFilter(): m_bits(65536, false), k_numHashes(32) {}
	BloomFilter(uint64_t m_bits, int numHashes) : m_bits(m_bits, false), k_numHashes(numHashes) {}
	void add(const void * key, size_t lenKey);/* key: DIP|SIP */
	bool contain(const void * key, size_t lenKey);
	inline std::array<uint64_t, 2> hashFunctionBF(const void * key, size_t lenKey) {
		std::array<uint64_t, 2>  hashValue;
		MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
		return hashValue;
	}

	inline uint64_t nthHashFunctionBF(int nth, uint64_t hashA, uint64_t hashB, uint64_t filterSize) {
		return (hashA + nth*hashB) % filterSize;
	}
};



#endif /* BLOOMFILTER_H_ */
