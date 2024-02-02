#ifndef MAC_H
#define MAC_H
#include "UnifiedTable.h"
class MAC :public UnifiedTable<MAC_ADD, PORT>
{
public:
	MAC(vector<pair<MAC_ADD, PORT>>_table, int& result, double _diamond_ratio = 1.92, uint32_t _bipartite = 0) :UnifiedTable<MAC_ADD, PORT>(_diamond_ratio)
	{
		LAYERS = ceil(log2(MAX_MAC_PORT + 1));
		des.resize(LAYERS);
		vector<vector<pair<MAC_ADD, bool>>> layer_table(LAYERS);
		int cnt = 0;
		for (auto& p : _table)
		{
			MAC_ADD item = p.first;
			PORT port = p.second;
			// from high bit to low bit
			for (size_t i = 0; i < LAYERS; i++)
			{
				int t = ACCESS8(port.array, 1, i);
				layer_table[i].push_back(make_pair(item, t));
			}
			cnt++;
		}
		result = 0;
		for (uint32_t i = 0; i < LAYERS; i++)
		{
			des[i] = new DiamondEmbedding<MAC_ADD>(calc_de_memory(diamond_ratio, layer_table[i].size()), _bipartite);
			if (des[i]->build(layer_table[i]) == -1)
			{
				//cout << "Down in Layer " << i << " Size=" << layer_table[i].size() << endl;
				result = -1;
			}
		}
	};
	~MAC() {}
	int insert(pair<MAC_ADD, PORT> item)
	{
		bool success = true;
		for (size_t i = 0; i < LAYERS; i++)
		{
			if (des[i]->insert(item.first, ACCESS8(item.second.array, 1, i)) == -1)
			{
				success = false;
				break;
			}
		}
		if (success == false) return -1;
		else return 0;
	}
	void withdraw(pair<MAC_ADD, PORT> item)
	{
		for (size_t i = 0; i < LAYERS; i++)
		{
			des[i]->withdraw(item.first, ACCESS8(item.second.array, 1, i));
		}
	}
	virtual PORT query(MAC_ADD item) const
	{
		PORT ret;
		for (size_t i = 0; i < LAYERS; i++)
		{
			WRITE8(ret.array, 1, i, des[i]->query(item));
		}
		return ret;
	}
	virtual uint32_t get_onchip_memory() const
	{
		uint32_t sum = 0;
		for (size_t i = 0; i < LAYERS; i++)
		{
			sum += des[i]->get_onchip_memory();
		}
		return sum;
	}
	virtual string get_info() const
	{
		stringstream out;
		out << "MAC TABLE INFO\n";
		for (size_t i = 0; i < LAYERS; i++)
		{
			out << "LAYER " << i << " " << *des[i] << endl;
		}
		return out.str();
	}
	uint32_t LAYERS;
private:
};
class SlightMAC :public SlightUnifiedTable<MAC_ADD, PORT>
{
public:
	SlightMAC(MAC* mather_table) :SlightUnifiedTable<MAC_ADD, PORT>(mather_table), LAYERS(mather_table->LAYERS) {};
	virtual PORT query(MAC_ADD item) const
	{
		PORT ret;
		for (size_t i = 0; i < LAYERS; i++)
		{
			WRITE8(ret.array, 1, i, des[i]->query(item));
		}
		return ret;
	}
	virtual uint32_t get_onchip_memory() const
	{
		uint32_t sum = 0;
		for (size_t i = 0; i < LAYERS; i++)
		{
			sum += des[i]->get_onchip_memory();
		}
		return sum;
	}
	uint32_t LAYERS;
};
#endif // !MAC_H