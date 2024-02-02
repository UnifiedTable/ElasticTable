#pragma once
#ifndef DE_H
#define DE_H
#include "../common/BOBHash32.h"
#include "../common/packed_array.h"
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<vector>
#include<stack>
using namespace std;
#define CHECK(x,y,reverse,z) (((((reverse) ? ((x) - (y)) : ((y) - (x))) & 2) >> 1) ^ (z) ^ 1)
bool compare_fun(pair<uint32_t, uint32_t>& a, pair<uint32_t, uint32_t>& b)
{
	return a.first < b.first;
}
template<class DATA_TYPE>
class DiamondEmbedding
{
	inline uint32_t hash32(const DATA_TYPE& item, uint32_t seed = 0)const {
		int t = sizeof(DATA_TYPE);
		//return Hash::BOBHash32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
		//if (seed == 0) return crc32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
		//else return truncate((uint8_t*)&item, sizeof(DATA_TYPE));
		if (seed == 0) return crc32((uint8_t*)&item, sizeof(DATA_TYPE), 998242353);
		else return crc32((uint8_t*)&item, sizeof(DATA_TYPE), 1e9 + 7);
	}
	pair<uint32_t, uint32_t> hash(const DATA_TYPE& item) const
	{
		uint32_t x, y;
		if (bipartite)
		{
			x = hash32(item, 0) % HALF_LENGTH;
			y = hash32(item, 1) % HALF_LENGTH + HALF_LENGTH;
		}
		else
		{
			x = hash32(item, 0) % LENGTH;
			y = hash32(item, 1) % LENGTH;
			if (x == y) y = (y + 1) % LENGTH;
		}
		return make_pair(x, y);
	}
	struct Edge
	{
		int x, y, z;
		Edge() { x = y = z = 0; }
		Edge(int _x, int _y, int _z) :x(_x), y(_y), z(_z) {};
		bool operator == (Edge y) const {
			return this->x == y.x && this->y == y.y && this->z == y.z;
		}
	};

public:
	DiamondEmbedding(uint32_t memoryInByte, uint32_t _bipartite = 0, double _max_conflict_ratio = 0.01)
	{
		HALF_LENGTH = memoryInByte * 8 / 2 / 2;
		LENGTH = HALF_LENGTH * 2;
		color = new uint8_t[memoryInByte];
		memset(color, 0, memoryInByte);
		map = vector<vector<Edge>>(LENGTH, vector<Edge>{});
		fail = 0;
		bipartite = _bipartite;
		MAC_CONFLICT_RATIO = _max_conflict_ratio;
	}
	~DiamondEmbedding()
	{
		delete[] color;
	}
	int check_compatible(uint32_t x, uint32_t y, bool category)
	{
		Edge target = Edge(x, y, category), target1 = Edge(y, x, 1 - category), conflict = Edge(x, y, 1 - category), conflict1 = Edge(y, x, category);
		if (find(map[x].begin(), map[x].end(), target) != map[x].end() || find(map[x].begin(), map[x].end(), target1) != map[x].end())
		{
			return 1;
		}
		if (find(map[x].begin(), map[x].end(), conflict) != map[x].end() || find(map[x].begin(), map[x].end(), conflict1) != map[x].end())
		{
			return -1;
		}
		return 0;
	}
	int build(vector<pair<DATA_TYPE, bool>> items)
	{
		int cnt1 = 0;
		for (auto& p : items)
		{
			DATA_TYPE item = p.first;
			bool category = p.second;
			auto hsh = hash(item);
			uint32_t x = hsh.first, y = hsh.second;
			switch (check_compatible(x, y, category))
			{
			case -1: {
				cnt1++;
				continue;
			}
			case 1: {
				continue; // compatible
			}
			default: {
				// do not appear befor
				Edge target = Edge(x, y, category);
				map[x].push_back(target);
				map[y].push_back(target);
			}
			}
		}
		if (cnt1 > ceil(MAC_CONFLICT_RATIO * items.size()) + 1)
		{
			return cnt1;
		}
		while (!coloring_stack.empty())
		{
			coloring_stack.pop();
		}
		visit.resize(LENGTH, false);
		//build stack
		unordered_map<uint32_t, vector<Edge>> mp;
		vector<uint32_t> points;
		for (size_t i = 0; i < LENGTH; i++)
		{
			mp[i] = map[i];
			points.push_back(i);
		}
		if (dg(-1, mp, points) == -1)
		{
			return -1;
		}
		visit = vector<bool>(LENGTH, false);

		while (!coloring_stack.empty())
		{
			auto p = coloring_stack.top();
			int32_t x = p.first, y = p.second;
			coloring_stack.pop();
			visit[x] = 1;
			bool finish = false;
			switch (y)
			{
			case -2:
			{
				finish = true;
				break;//degree 0 or cut vertex
			}
			case -1:
			{
				//degree 1
				for (auto& edge : map[x])
				{
					int32_t z = edge.x ^ edge.y ^ x;
					// x-z
					if (visit[z] == 0) continue;
					uint32_t color_z = ACCESS8(color, 2, z);
					for (size_t color_x = 0; color_x < 4; color_x++)
					{
						if (CHECK(color_x, color_z, x == edge.y, edge.z))
						{
							WRITE8(color, 2, x, color_x);
							finish = true;
							break;
						}
					}
					break;
				}
				break;
			}
			default:
			{
				//degree 2
				visit[y] = 1;
				Edge e = map[x][0], front_e = map[x][0], tail_e = map[x][0];
				for (auto& edge : map[x])
				{
					int z = edge.y ^ edge.x ^ x;
					if (z == y)
					{
						e = edge;
					}
					else if (visit[z] == 1)
					{
						front_e = edge;
					}
				}
				for (auto& edge : map[y])
				{
					int z = edge.y ^ edge.x ^ y;
					if (visit[z] == 1 && z != x)
					{
						tail_e = edge;
					}
				}
				//p-x-y-q
				uint32_t p = front_e.x ^ front_e.y ^ x, q = tail_e.x ^ tail_e.y ^ y;
				uint32_t color_p = ACCESS8(color, 2, p), color_q = ACCESS8(color, 2, q);
				for (size_t color_x = 0; color_x < 4 && finish == false; color_x++)
				{
					if (CHECK(color_p, color_x, p == front_e.y, front_e.z))
					{
						for (size_t color_y = 0; color_y < 4 && finish == false; color_y++)
						{
							if (CHECK(color_x, color_y, x == e.y, e.z) && CHECK(color_y, color_q, y == tail_e.y, tail_e.z))
							{
								WRITE8(color, 2, x, color_x);
								WRITE8(color, 2, y, color_y);
								finish = true;
								break;
							}
						}
					}
				}
			}
			}
			if (finish == false)
			{
				fail++;
				return -1;
			}
		}
		for (size_t x = 0; x < LENGTH; x++)
		{
			int color_x = ACCESS8(color, 2, x);
			for (auto& edge : map[x])
			{
				int y = edge.y ^ edge.x ^ x;
				int color_y = ACCESS8(color, 2, y);
				if (!CHECK(color_x, color_y, x == edge.y, edge.z))
				{
					fail++;
					return -1;
				}
			}
		}
		visit = vector<bool>(LENGTH, false);
		return 0;
	}
	int insert(DATA_TYPE item, bool category)
	{
		//¶¯Ì¬È¾É«
		auto hsh = hash(item);
		uint32_t x = hsh.first, y = hsh.second;

		switch (check_compatible(x, y, category))
		{
		case -1: {
			fail++;
			return -1; // conflict
		}
		case 1: {
			return 0; // compatible
		}
		default: {
			// do not appear befor
			Edge target = Edge(x, y, category);
			map[x].push_back(target);
			map[y].push_back(target);
			iter = 0;
			int ret = (map[x].size() < map[y].size()) ? coloring(y) : coloring(x);
			if (ret == -1)
			{
				fail++;
			}
			return ret;
		}
		}
	}
	int query(const DATA_TYPE item) const
	{
		auto hsh = hash(item);
		uint32_t x = hsh.first, y = hsh.second;
		return CHECK(ACCESS8(color, 2, x), ACCESS8(color, 2, y), 0, 1);
	}
	void withdraw(DATA_TYPE item, bool category)
	{
		auto hsh = hash(item);
		uint32_t x = hsh.first, y = hsh.second;
		Edge target(x, y, category);
		for (auto ptr = map[x].begin(); ptr != map[x].end(); ptr++)
		{
			if ((*ptr) == target)
			{
				map[x].erase(ptr);
				break;
			}
		}
		for (auto ptr = map[y].begin(); ptr != map[y].end(); ptr++)
		{
			if ((*ptr) == target)
			{
				map[y].erase(ptr);
				break;
			}
		}
	}
	void get_color_array(uint8_t*& target, uint32_t& length) const
	{
		uint32_t arr_length = LENGTH * 2 / 8;
		target = new uint8_t[arr_length];
		copy(color, color + arr_length, target);
	}
	uint16_t get_color(uint32_t pos) const
	{
		return ACCESS8(color, 2, pos);
	}
	uint32_t get_onchip_memory() const
	{
		return LENGTH * 2 / 8;
	}
	friend ostream& operator << (ostream& os, const DiamondEmbedding& a)
	{
		os << "LENGTH=" << a.LENGTH << " ";
		for (size_t i = 0; i < a.LENGTH; i++)
		{
			os << a.get_color(i) << ",";
		}
		return os;
	}
	uint32_t LENGTH, HALF_LENGTH, bipartite;
private:
	int coloring(int x)
	{
		stack<pair<int, int>>history;
		queue<int> v;
		v.push(x);
		while (!v.empty() && history.size() < 10000)
		{
			x = v.front();
			for (auto& edge : map[x])
			{
				uint32_t y = edge.x ^ edge.y ^ x;
				uint8_t color_x = ACCESS8(color, 2, x), color_y = ACCESS8(color, 2, y);
				if (CHECK(color_x, color_y, x == edge.y, edge.z)) continue;
				int optimal = -1, mincost = 0x7fffffff;
				for (uint32_t new_color_y = 0; new_color_y < 4; new_color_y++)
				{
					if (!CHECK(color_x, new_color_y, x == edge.y, edge.z)) continue;
					int cost = 0;
					for (auto& edge1 : map[y])
					{
						uint32_t w = edge1.x ^ edge1.y ^ y;
						cost += (1 - CHECK(new_color_y, ACCESS8(color, 2, w), y == edge1.y, edge1.z)) * map[w].size();
					}
					//if (optimal == -1 || (rand() % 2 == 0))
					if (cost < mincost)
					{
						optimal = new_color_y;
						mincost = cost;
					}
				}
				if (optimal != -1)
				{
					history.push(make_pair(y, color_y));
					WRITE8(color, 2, y, optimal);
					v.push(y);
				}
				else
				{
					break;
				}
			}
			v.pop();
		}
		if (v.empty())
		{
			return history.size();
		}
		else
		{
			while (!history.empty())
			{
				auto p = history.top();
				WRITE8(color, 2, p.first, p.second);
				history.pop();
			}
			return -1;
		}
	}
	int dg(int32_t root, unordered_map<uint32_t, vector<Edge>>& mp, const vector<uint32_t>& points)
	{
		uint32_t cnt = 0;
		queue<uint32_t> check_list;
		if (root >= 0)
		{
			cnt = 1;
		}
		for (auto& i : points)
		{
			if (visit[i]) continue;
			judge(root, i, check_list, cnt, mp);
		}
		while (!check_list.empty())
		{
			uint32_t x = check_list.front();
			check_list.pop();
			for (auto& edge : mp[x])
			{
				uint32_t y = edge.x ^ edge.y ^ x;
				if (visit[y]) continue;
				judge(root, y, check_list, cnt, mp);
			}
		}
		if (cnt != points.size())
		{
			// find new cut vertex
			queue<pair<int32_t, pair<unordered_map<uint32_t, vector<Edge>>, vector<uint32_t>>>> components;
			vector<uint32_t> roots;
			unordered_map<uint32_t, uint32_t> dfn, low;
			for (auto& i : points)
			{
				if (visit[i] || dfn[i]) continue;
				int before = components.size();
				num = 0;
				tarjan(i, -1, dfn, low, mp, components);
				//tarjan(i, -1, mp, components);
				roots.push_back(i);
				if (components.size() - before == 1)
				{
					return -1;
				}
			}
			while (!components.empty())
			{
				auto p = components.front();
				components.pop();
				if (dg(p.first, p.second.first, p.second.second) == -1)
				{
					return -1;
				}
			}
			for (auto& root : roots)
			{
				visit[root] = true;
				coloring_stack.push(make_pair(root, -2));
			}
		}
		return 0;
	}
	int tarjan(int x, int father, unordered_map<uint32_t, uint32_t>& dfn, unordered_map<uint32_t, uint32_t>& low,
		unordered_map<uint32_t, vector<Edge>>& mp, queue< pair<int32_t, pair<unordered_map<uint32_t, vector<Edge>>, vector<uint32_t>>>>& components)
	{
		unordered_set<uint32_t> recorded;
		unordered_map<uint32_t, uint32_t> parent;
		stack<pair<int, int>> v;
		v.push(make_pair(x, father));
		stack<int> component_stack;
		int num = 0, sum = 0;
		while (!v.empty())
		{
			auto p = v.top();
			x = p.first;
			father = p.second;
			if (dfn[x] == 0)
			{
				dfn[x] = low[x] = ++num;
				component_stack.push(x);
			}
			bool finish = true;
			for (auto& edge : mp[x])
			{
				int y = edge.x ^ edge.y ^ x;
				if (y == father || visit[y]) continue;
				int p = low[y], q = dfn[x];
				if (dfn[y] == 0)
				{
					v.push(make_pair(y, x));
					parent[y] = x;
					finish = false;
					break;
				}
				else if (parent[y] == x)
				{
					low[x] = min(low[x], low[y]);
					if (low[y] >= dfn[x] && recorded.find(y) == recorded.end())
					{
						vector<uint32_t> component;
						unordered_set<uint32_t> dict;
						unordered_map<uint32_t, vector<Edge>> mp1;
						while (true)
						{
							int p = component_stack.top();
							component.push_back(p);
							dict.insert(p);
							if (p == x) break;
							component_stack.pop();
							recorded.insert(p);
						}
						for (auto& point : component)
						{
							auto w = mp[point];
							for (auto& edge : mp[point])
							{
								int y = edge.x ^ edge.y ^ point;
								if (dict.find(y) != dict.end())
								{
									mp1[point].push_back(edge);
								}
							}
						}
						sum += component.size();
						components.push(make_pair(x, make_pair(mp1, component)));
					}
				}
				else
				{
					low[x] = min(low[x], dfn[y]);
				}
			}
			if (finish)
			{
				v.pop();
			}
		}
		return sum;
	}
	int get_degree(int x, unordered_map<uint32_t, vector<Edge>>& mp)
	{
		int ret = 0;
		for (auto& edge1 : mp[x])
		{
			uint32_t z = edge1.x ^ edge1.y ^ x;
			ret += 1 - visit[z];
		}
		return ret;
	}
	void judge(uint32_t root, uint32_t x, queue<uint32_t>& check_list, uint32_t& cnt, unordered_map<uint32_t, vector<Edge>>& mp)
	{
		if (x == root) return;
		uint32_t degree = get_degree(x, mp);
		switch (degree)
		{
		case 0:
		{
			coloring_stack.push(make_pair(x, -2));
			visit[x] = 1;
			cnt++;
			break;
		}
		case 1:
		{
			coloring_stack.push(make_pair(x, -1));
			check_list.push(x);
			visit[x] = 1;
			cnt++;
			break;
		}
		case 2:
		{
			vector<Edge> t = mp[x];
			for (auto& edge : t)
			{
				int y = edge.x ^ edge.y ^ x;
				if (get_degree(y, mp) == 2 && visit[y] == 0 && y != root)
				{
					visit[y] = 1;
					visit[x] = 1;
					check_list.push(y);
					check_list.push(x);
					coloring_stack.push(make_pair(x, y));
					cnt += 2;
					break;
				}
			}
			break;
		}
		default:
			break;
		}
	}
	uint32_t iter;
	uint32_t fail;
	vector<vector<Edge>> map;
	vector<bool> visit;
	stack<pair<int32_t, int32_t>> coloring_stack;
	int num;
	stack<int> component_stack;
	double MAC_CONFLICT_RATIO;
	uint8_t* color;
};
template<class DATA_TYPE>
class SlightDiamondEmbedding {
	inline uint32_t hash32(const DATA_TYPE& item, uint32_t seed = 0)const {
		int t = sizeof(DATA_TYPE);
		//return Hash::BOBHash32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
		//if (seed == 0) return crc32((uint8_t*)&item, sizeof(DATA_TYPE), seed);
		//else return truncate((uint8_t*)&item, sizeof(DATA_TYPE));
		if (seed == 0) return crc32((uint8_t*)&item, sizeof(DATA_TYPE), 998242353);
		else return crc32((uint8_t*)&item, sizeof(DATA_TYPE), 1e9 + 7);
	}
	pair<uint32_t, uint32_t> hash(const DATA_TYPE& item) const
	{
		uint32_t x, y;
		if (bipartite)
		{
			x = hash32(item, 0) % HALF_LENGTH;
			y = hash32(item, 1) % HALF_LENGTH + HALF_LENGTH;
		}
		else
		{
			x = hash32(item, 0) % LENGTH;
			y = hash32(item, 1) % LENGTH;
			if (x == y) y++;
		}
		return make_pair(x, y);
	}
public:
	SlightDiamondEmbedding(const DiamondEmbedding<DATA_TYPE>* de)
	{
		HALF_LENGTH = de->HALF_LENGTH;
		bipartite = de->bipartite;
		de->get_color_array(color, LENGTH);
	}
	~SlightDiamondEmbedding()
	{
		delete[] color;
	}
	int query(DATA_TYPE item) const
	{
		auto hsh = hash(item);
		uint32_t x = hsh.first, y = hsh.second;
		return CHECK(ACCESS8(color, 2, x), ACCESS8(color, 2, y), 0, 1);
	}
	virtual uint32_t get_onchip_memory() const
	{
		return LENGTH * 2 / 8;
	}
	void get_color(uint8_t*& target, uint32_t& length) const
	{
		uint32_t arr_length = LENGTH * 2 / 8;
		target = new uint8_t[arr_length];
		copy(color, color + arr_length, target);
	}
	uint8_t* color;
	uint32_t LENGTH, HALF_LENGTH;
	int bipartite;
};
#endif // !DE_H
