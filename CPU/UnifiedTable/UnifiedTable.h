#ifndef UNIFIED_H
#define UNIFIED_H
#include "DiamondEmbedding.h"
#include "../common/universal.h"
template <class DATA_TYPE, class ENTRY_TYPE> // MAC 就是port,ACL就是二进制规则，LPM就是下一跳
class UnifiedTable
{
	inline uint32_t hash32(const DATA_TYPE& item, uint32_t seed = 0)const {
		return Hash::BOBHash32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
	}
public:
	UnifiedTable(double _diamond_ratio) :diamond_ratio(_diamond_ratio) {}
	~UnifiedTable() {
		for (auto& ptr : des)
		{
			delete ptr;
		}
	}
	void get_slight_embedding(vector<SlightDiamondEmbedding<DATA_TYPE>*>& sdes)
	{
		for (auto& de : des)
		{
			sdes.push_back(new SlightDiamondEmbedding<DATA_TYPE>(de));
		}
	}
	// 0success -1fail
	virtual int insert(pair<DATA_TYPE, ENTRY_TYPE> item) = 0;
	virtual void withdraw(pair<DATA_TYPE, ENTRY_TYPE> item) = 0;
	virtual ENTRY_TYPE query(DATA_TYPE item) const = 0;
	virtual uint32_t get_onchip_memory() const = 0;
	virtual string get_info() const = 0;
	friend ostream& operator<<(ostream& os, const UnifiedTable& a)
	{
		os << a.get_info() << endl;
		return os;
	}
protected:
	vector<DiamondEmbedding<DATA_TYPE>*> des;
	double diamond_ratio;
};
template <class DATA_TYPE, class ENTRY_TYPE>
class SlightUnifiedTable
{
	inline uint32_t hash32(const DATA_TYPE& item, uint32_t seed = 0)const {
		return Hash::BOBHash32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
	}
public:
	SlightUnifiedTable(UnifiedTable<DATA_TYPE, ENTRY_TYPE>* mathor_table) :off_chip(mathor_table)
	{
		mathor_table->get_slight_embedding(des);
	}
	~SlightUnifiedTable() {
		for (auto& ptr : des)
		{
			delete ptr;
		}
	}
	virtual ENTRY_TYPE query(DATA_TYPE item) const = 0;
	virtual uint32_t get_onchip_memory() const = 0;
protected:
	vector<SlightDiamondEmbedding<DATA_TYPE>*> des;
	UnifiedTable<DATA_TYPE, ENTRY_TYPE>* off_chip;
};
#endif // !UNIFIED_H