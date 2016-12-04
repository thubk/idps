#ifndef BUCKET_H_
#define BUCKET_H_
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "MurmurHash3.h"
using namespace std;

class Bucket {
private:
	uint32_t owner;
	int k_numHashes;/*  */
	int m_bits; /* size of bitmap */
	uint32_t counter;
	uint32_t distNum; /*  */
	std::vector<std::vector<bool> > L_bits; /* PCSA */
	std::vector<int> M; /* HyperLogLog */
public:
	/* k = 128, 23 bits memory: 2^32 IP address , 1 hash functions */
	Bucket() :
			owner(0), k_numHashes(128), m_bits(23), counter(0), distNum(0), L_bits(
					k_numHashes, vector<bool>(m_bits, false)), M(k_numHashes, 0) {
	}
	~Bucket() {
	}
	void increaseCounter();/* +1 */
	void decreaseCounter();/* -1 */
	void setOwner(uint32_t key);
	uint32_t getCounter();
	uint32_t getDistNum();
	uint32_t getOwner();
	void estimatePCSA64(const void * key, size_t lenKey); /* PCSA */
	void estimatePCSA32(uint32_t key);
	void estimateHLL64(const void * key, size_t lenKey); /* HyperLogLog */
	void estimateHLL32(uint32_t key);
	inline std::array<uint64_t, 2> hashFunctionBucket(const void * key,
			size_t lenKey) {
		std::array<uint64_t, 2> hashValue;
		MurmurHash3_x64_128(key, lenKey, 0, hashValue.data());
		return hashValue;
	}

	inline uint64_t nthHashFunctionBucket(int nth, uint64_t hashA,
			uint64_t hashB) {
		return hashA + nth * hashB;
	}

	/* Rightmost bit'1 *//* 64 bit */
	inline int getFirstIndex64(uint64_t hashValue, int k_numHashes) {
		int index = -1;
		int radix = log2(k_numHashes);
		uint64_t temp = hashValue >> radix;
		for (int i = 0; i < (64 - radix); i++) {
			if ((temp & (1 << (64 - radix - 1 - i))) != 0) {
				index = i;
				break;
			}
		}
		return index;
	}
	/* 32 bit */
	inline int getFirstIndex32(uint32_t hashValue, int k_numHashes) {
		int index = -1;
		int radix = log2(k_numHashes);
		uint64_t temp = hashValue >> radix;
		for (int i = 0; i < (32 - radix); i++) {
			if ((temp & (1 << (32 - radix - 1 - i))) != 0) {
				index = i;
				break;
			}
		}
		return index;
	}
};

#endif /* BUCKET_H_ */
