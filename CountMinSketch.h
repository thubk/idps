/* MurmurHash - MIT */
#ifndef COUNTMINSKETCH_H_
#define COUNTMINSKETCH_H_
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "BloomFilter.h"
#include "Bucket.h"
#include "MurmurHash3.h"
using namespace std;

/* Counter Min sketch (2 dims) - Bidirectional Min Sketch */

class CountMinSketch {
private:
	uint8_t k_numHashes;
	std::vector<BloomFilter> bf_row;/* bloomfilter */
	std::vector<std::vector<Bucket> > m_bits_row;/*  bucket */
public:
	CountMinSketch(uint64_t m_bits, uint8_t numHashes) :
			k_numHashes(numHashes), m_bits_row(numHashes,
					vector<Bucket>(m_bits, Bucket())), bf_row(numHashes,
					BloomFilter()) {
	}
	void process(const void * key1, const void * key2, const void * key3,
			size_t lenKey, uint32_t key);/* key1: DIP key2: SIP */
	void update(const void * key1, const void * key2, size_t lenKey);
	bool contain(const void * key, size_t lenKey);
	uint16_t getMinFrequence(const void * key, size_t lenKey);

	uint8_t getHashIndex(const void * key1, size_t lenKey, bool flag,
			uint32_t key);

	uint32_t getDistNumBCS(const void * key1, size_t lenKey, uint32_t key);

	inline std::array<uint64_t, 2> hashFunctionBCS(const void * key,
			size_t lenKey) {
		std::array<uint64_t, 2> hashValue;
		MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
		return hashValue;
	}

	inline uint64_t nthHashFunctionBCS(uint8_t nth, uint64_t hashA,
			uint64_t hashB, uint64_t filterSize) {
		return (hashA + nth * hashB) % filterSize;
	}
};

#endif /* COUNTMINSKETCH_H_ */
