/* MurmurHash - MIT */
#ifndef BICOUNTSKETCH_H_
#define BICOUNTSKETCH_H_
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

/* Counter Min sketch (2 dims) - Bidirectional Count Sketch */

class BiCountSketch {
private:
	int k_numHashes;
	std::vector<BloomFilter> bf_row;/* bloomfilter */
	std::vector<std::vector<Bucket> > m_bits_row;/*  bucket */
public:
	BiCountSketch(uint64_t m_bits, int numHashes) :
			k_numHashes(numHashes), m_bits_row(numHashes,
					vector<Bucket>(m_bits, Bucket())), bf_row(numHashes,
					BloomFilter()) {
	}
	void process(const void * key1, const void * key2, const void * key3,
			size_t lenKey, uint32_t key);/* key1: DIP key2: SIP */
	void update(const void * key1, const void * key2, size_t lenKey);
	bool contain(const void * key, size_t lenKey);
	uint32_t getMinFrequence(const void * key, size_t lenKey);

	int getHashIndex(const void * key1, size_t lenKey, bool flag,
			uint32_t key);

	uint32_t getDistNumBCS(const void * key1, size_t lenKey, uint32_t key);

	int getHashIndexBucket(const void * key1, size_t lenKey, uint32_t key);
	uint32_t getSumHashesRow(int index);



	inline std::array<uint64_t, 2> hashFunctionBCS(const void * key,
			size_t lenKey) {
		std::array<uint64_t, 2> hashValue;
		MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
		return hashValue;
	}

	inline uint64_t nthHashFunctionBCS(int nth, uint64_t hashA,
			uint64_t hashB, uint64_t filterSize) {
		return (hashA + nth * hashB) % filterSize;
	}
};

#endif /* BICOUNTSKETCH_H_ */
