#ifndef COUNTMINSKETCH_H_
#define COUNTMINSKETCH_H_
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "BucketCMS.h"
#include "MurmurHash3.h"
using namespace std;

class CountMinSketch {
private:
	uint8_t k_numHashes;
	std::vector<std::vector<BucketCMS> > m_bits_row;
public:
	CountMinSketch(uint64_t m_bits, uint8_t numHashes) :
			k_numHashes(numHashes), m_bits_row(numHashes,
					vector<BucketCMS>(m_bits, BucketCMS())) {
	}

	void process(const void * key1, size_t lenKey, uint32_t key);
	bool contain(const void * key1, size_t lenKey, uint32_t key);

	uint8_t getHashIndex(const void * key1, size_t lenKey, uint32_t key);
	inline std::array<uint64_t, 2> hashFunctionCMS(const void * key,
			size_t lenKey) {
		std::array<uint64_t, 2> hashValue;
		MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
		return hashValue;
	}

	inline uint64_t nthHashFunctionCMS(uint8_t nth, uint64_t hashA,
			uint64_t hashB, uint64_t filterSize) {
		return (hashA + nth * hashB) % filterSize;
	}
};

#endif /* COUNTMINSKETCH_H_ */
