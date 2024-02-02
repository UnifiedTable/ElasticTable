#pragma once
#ifndef NORMAL_HASH_H
#define NORMAL_HASH_H
#include"../common/universal.h"
template<class DATA_TYPE, class ENTRY_TYPE>
class NormalHash
{
private:
	struct node {
		ENTRY_TYPE entry;
		bool valid;
	};
	int LENGTH;
	vector<node> buckets;
	inline uint32_t hash32(const DATA_TYPE& item, uint32_t seed = 0)const {
		return Hash::BOBHash32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
	}
public:
	NormalHash(uint32_t memoryInByte, uint32_t PORT_BIT)
	{
		LENGTH = memoryInByte * 8 / (1 + PORT_BIT);
		buckets.resize(LENGTH);
	}

	~NormalHash() {}
	int insert(pair<DATA_TYPE, ENTRY_TYPE> item)
	{
		uint32_t idx = hash32(item.first, 0) % LENGTH;
		if (buckets[idx].valid == true) return -1;
		buckets[idx].entry = item.second;
		buckets[idx].valid = true;
		return 0;
	}
};
#endif // !NORMAL_HASH_H