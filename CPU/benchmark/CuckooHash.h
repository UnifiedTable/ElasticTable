#ifndef CUCKOO_H
#define CUCKOO_H
#include"../common/universal.h"
#include<vector>
template<class DATA_TYPE, class ENTRY_TYPE>
class CuckooHash
{
	inline uint32_t hash32(const DATA_TYPE& item, uint32_t seed = 0)const {
		return Hash::BOBHash32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
	}
	template<class T>
	inline void swap(T& a, T& b) {
		T c = a;
		a = b;
		b = c;
	}
public:
	CuckooHash(uint32_t memoryInByte, uint32_t PORT_BIT)
	{
		LENGTH = 1 << int(log2(memoryInByte * 8.0 / (8 + 1 + PORT_BIT)));
		buckets.resize(LENGTH);
	}
	~CuckooHash() {}
	int insert(pair<DATA_TYPE, ENTRY_TYPE> item)
	{
		uint8_t fp = hash32(item.first, 1) & 0xff;
		uint32_t idx0 = hash32(item.first, 0) % LENGTH;
		ENTRY_TYPE entry = item.second;
		int iter = 0;
		while (iter++ < 10000)
		{
			if (buckets[idx0].item == false)
			{
				if (buckets[idx0].item)
				{
					int t = 1;
				}
				buckets[idx0] = { fp, entry ,true };
			}
			if (buckets[idx0].item == fp)
			{
				return 0;
			}
			else
			{
				swap(fp, buckets[idx0].item);
				swap(entry, buckets[idx0].entry);
				idx0 ^= Hash::BOBHash32(&fp, 1, 2) % LENGTH;
			}
		}
		return -1;
	}
private:
	struct node {
		uint8_t item;
		ENTRY_TYPE entry;
		bool valid;
	};
	int LENGTH;
	vector<node> buckets;
};
#endif // !CUCKOO_H
