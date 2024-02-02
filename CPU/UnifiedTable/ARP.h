#ifndef ARP_H
#define ARP_H
#include "UnifiedTable.h"
class ARP :public UnifiedTable<IP, MAC_ADD>
{
public:
	ARP(vector<pair<IP, MAC_ADD>>_table, int& result, double _diamond_ratio = 1.92, uint32_t _bipartite = 0) :UnifiedTable<IP, MAC_ADD>(_diamond_ratio)
	{
		LAYERS = IP_LEN;
		des.resize(LAYERS);
		vector<vector<pair<IP, bool>>> layer_table(LAYERS);
		int cnt = 0;
		for (auto& p : _table)
		{
			IP item = p.first;
			MAC_ADD MAC_ADD = p.second;
			// from high bit to low bit
			for (size_t i = 0; i < LAYERS; i++)
			{
				int t = ACCESS8(MAC_ADD.array, 1, i);
				layer_table[i].push_back(make_pair(item, t));
			}
			cnt++;
		}
		result = 0;
		for (uint32_t i = 0; i < LAYERS; i++)
		{
			des[i] = new DiamondEmbedding<IP>(calc_de_memory(diamond_ratio, layer_table[i].size()), _bipartite);
			if (des[i]->build(layer_table[i]) == -1)
			{
				//cout << "Down in Layer " << i << " Size=" << layer_table[i].size() << endl;
				result = -1;
			}
		}
	};
	~ARP() {}
	int insert(pair<IP, MAC_ADD> item)
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
	void withdraw(pair<IP, MAC_ADD> item)
	{
		for (size_t i = 0; i < LAYERS; i++)
		{
			des[i]->withdraw(item.first, ACCESS8(item.second.array, 1, i));
		}
	}
	virtual MAC_ADD query(IP item) const
	{
		MAC_ADD mac;
		for (size_t i = 0; i < LAYERS; i++)
		{
			WRITE8(mac.array, 1, i, des[i]->query(item));
		}
		return mac;
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
		out << "ARP TABLE INFO\n";
		for (size_t i = 0; i < LAYERS; i++)
		{
			out << "LAYER " << i << " " << *des[i] << endl;
		}
		return out.str();
	}
	uint32_t LAYERS;
private:
};
class SlightARP :public SlightUnifiedTable<IP, MAC_ADD>
{
public:
	SlightARP(ARP* mather_table) :SlightUnifiedTable<IP, MAC_ADD>(mather_table), LAYERS(mather_table->LAYERS) {};
	virtual MAC_ADD query(IP item) const
	{
		MAC_ADD mac;
		for (size_t i = 0; i < LAYERS; i++)
		{
			WRITE8(mac.array, 1, i, des[i]->query(item));
		}
		return mac;
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
#endif // !ARP_H