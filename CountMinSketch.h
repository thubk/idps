#ifndef COUNTMINSKETCH_H_
#define COUNTMINSKETCH_H_
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "MurmurHash3.h"
using namespace std;

class CountMinSketch {
private:
	int k_numHashes;
	std::vector<std::vector<uint32_t> > m_bits_row;
public:
	CountMinSketch(uint64_t m_bits, int numHashes) :
			k_numHashes(numHashes), m_bits_row(numHashes,
					vector<uint32_t>(m_bits, 0)) {
	}

	void process(const void * key, size_t lenKey);
	bool contain(const void * key, size_t lenKey);
	void updateCounter(const void * key, size_t lenKey, uint32_t min);
	void resetCounter(const void * key, size_t lenKey, uint32_t min);
	uint32_t getMinCounter(const void * key, size_t lenKey);
	inline std::array<uint64_t, 2> hashFunctionCMS(const void * key,
			size_t lenKey) {
		std::array<uint64_t, 2> hashValue;
		MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
		return hashValue;
	}

	inline uint64_t nthHashFunctionCMS(int nth, uint64_t hashA,
			uint64_t hashB, uint64_t filterSize) {
		return (hashA + nth * hashB) % filterSize;
	}
};

#endif /* COUNTMINSKETCH_H_ */
