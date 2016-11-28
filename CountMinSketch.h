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

class CountMinSketch {
private:
	uint8_t k_numHashes;
	std::vector< std::vector<Bucket> > m_bits_row;/*  bucket */
	std::vector< std::vector<BloomFilter> > bf_row;/* bloomfilter */
public:
	CountMinSketch(uint64_t m_bits, uint8_t numHashes, Bucket bucket);
	void process(const uint8_t *key1,const uint8_t *key2, size_t lenKey);/* key1: DIP key2: SIP */
	bool contain(const uint8_t *key, size_t lenKey);
	uint16_t getMinFrequence(const uint8_t *key, size_t lenKey);
};



std::array<uint64_t, 2> hashFunctionBCS(const uint8_t *key, size_t lenKey) {
	 std::array<uint64_t, 2>  hashValue;
	MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
	return hashValue;
}

uint64_t nthHashFunctionBCS(uint8_t nth, uint64_t hashA, uint64_t hashB, uint64_t filterSize) {
	return (hashA + nth*hashB) % filterSize;
}

#endif /* COUNTMINSKETCH_H_ */
