#include <iostream>
#include "common/benchmark.h"
#include"UnifiedTable/DiamondEmbedding.h"
#include"UnifiedTable/ACL.h"
#include"UnifiedTable/FIB.h"
#include"UnifiedTable/MAC.h"
#include <chrono>
using namespace std;
const string folder[] = { "../../../datasets/MAC/" ,"../../../datasets/CAIDA/", };
const string filenames[] = { "complete_mac_table.txt" ,"trace3.dat","130000.dat" };
struct Edge
{
	int x, y, z;
	Edge() { x = y = z = 0; }
	Edge(int _x, int _y, int _z) :x(_x), y(_y), z(_z) {};
	bool operator == (Edge y) const {
		return this->x == y.x && this->y == y.y && this->z == y.z;
	}
};
pair<double, double> dg_DE(double bit_per_key, vector<pair<MAC_ADD, bool>> dataset, double static_ratio, uint32_t record_num = 0)
{
	vector<pair<MAC_ADD, bool>> build_items;
	DiamondEmbedding<MAC_ADD>* de = new DiamondEmbedding<MAC_ADD>(dataset.size() * bit_per_key / 8);
	uint32_t cnt = 0, fail = 0, cnt1 = 0;
	for (auto& p : dataset)
	{
		++cnt;
		if (cnt <= int(dataset.size() * static_ratio))
		{
			build_items.push_back(p);
			if (cnt == int(dataset.size() * static_ratio))
			{
				auto result = de->build(build_items);
				if (result == -1) return make_pair(0, 0);
				else if (result > 0) return make_pair(0, result * 1.0 / dataset.size());
			}
		}
		else
		{
			int ret = de->insert(p.first, p.second);
			if (cnt > dataset.size() - record_num)
			{
				if (ret == -1)
				{
					fail++;
				}
				else
				{
					cnt1 += ret;
				}
			}
		}
	}
	delete de;
	return make_pair(1 - fail * 1.0 / max(1u, record_num), cnt1 * 1.0 / max(1u, record_num - fail));
}
void test_staticDE(vector<pair<MAC_ADD, bool>> dataset, string prefix, double ratio_base, double ratio_step, double ratio_stop, int test_num, vector<uint32_t> scales)
{
	ofstream out;
	out.open("../result/DE_static_" + prefix + "_SuccessRate.csv");
	out << "MEM(KB)" << ",";
	for (auto& scale : scales)
	{
		out << scale << ",";
	}
	for (double ratio = ratio_base; ratio < ratio_stop; ratio += ratio_step)
	{
		std::cout << "bit per key=" << 2.0 / ratio << endl;
		out << ratio << ",";
		for (auto& scale : scales)
		{
			double cnt = 0, cnt1 = 0, cnt2 = 0;
			for (size_t i = 0; i < test_num; i++)
			{
				random_shuffle(dataset.begin(), dataset.end());
				vector<pair<MAC_ADD, bool>> tmp(dataset.begin(), dataset.begin() + scale * ratio);
				auto ret = dg_DE(2.0 / ratio, tmp, 1.0);
				cnt += ret.first;
				if (ret.second)
				{
					cnt2 += ret.second;
					cnt1++;
				}
			}
			cnt /= test_num;
			cnt2 /= max(1.0, cnt1);
			cnt1 /= test_num;
			std::cout << "scale=" << scale << " success_rate=" << cnt << " modify success_rate=" << cnt / (1 - cnt1)
				<< " conflict_rate=" << cnt1 << " average conflict edge=" << cnt2 << endl;
			out << cnt << ",";
		}
	}
	out.close();
}
void test_dynamicDE(vector<pair<MAC_ADD, bool>> dataset, string prefix, double ratio_base, double ratio_step, double ratio_stop, int test_num, vector<uint32_t> scales)
{
	ofstream out1, out2;
	out1.open("../result/DE_dynamic_" + prefix + "_SuccessRate.csv");
	out2.open("../result/DE_dynamic_" + prefix + "_AverageChange.csv");
	out1 << "MEM(KB)" << ",";
	out2 << "MEM(KB)" << ",";
	for (auto& scale : scales)
	{
		out1 << scale << ",";
	}
	for (double ratio = ratio_base; ratio < ratio_stop; ratio += ratio_step)
	{
		std::cout << "bit per key=" << 2.0 / ratio << endl;
		out1 << ratio << ",";
		out2 << ratio << ",";
		for (auto& scale : scales)
		{
			double cnt = 0, cnt1 = 0;
			for (size_t i = 0; i < test_num; i++)
			{
				random_shuffle(dataset.begin(), dataset.end());
				vector<pair<MAC_ADD, bool>> tmp(dataset.begin(), dataset.begin() + scale * ratio);
				auto ret = dg_DE(2.0 / ratio, tmp, ratio_base / ratio, ratio_step * scale);
				cnt += ret.first; cnt1 += ret.second;
			}
			cnt /= test_num;
			cnt1 /= test_num;
			std::cout << "scale=" << scale << " success_rate=" << cnt << " avg_change=" << cnt1 << endl;
			out1 << cnt << ",";
			out2 << cnt1 << ",";
		}
	}
	out1.close(); out2.close();
}
void test_staticDE_truthMAC(const char* PATH, double ratio_base, double ratio_step, double ratio_stop, int test_num, vector<uint32_t> scales)
{
	std::cout << " DE static Benchmark file=" << PATH << std::endl << std::endl;
	count_type cnt;
	vector<MAC_ADD> items = read_mac(PATH, cnt);
	std::cout << "The number of items:" << cnt << std::endl;
	vector<pair<MAC_ADD, bool>> dataset;
	for (auto& mac : items)
	{
		dataset.push_back(make_pair(mac, hash32(mac, 2) & 1));
	}
	test_staticDE(dataset, "truth", ratio_base, ratio_step, ratio_stop, test_num, scales);
}
void test_staticDE_randomMAC(int N, double ratio_base, double ratio_step, double ratio_stop, int test_num, vector<uint32_t> scales)
{
	std::cout << " DE static Benchmark random" << std::endl << std::endl;
	vector<MAC_ADD> items = generate_mac(N);
	vector<pair<MAC_ADD, bool>> dataset;
	for (auto& mac : items)
	{
		dataset.push_back(make_pair(mac, hash32(mac, 2) & 1));
	}
	test_staticDE(dataset, "generate", ratio_base, ratio_step, ratio_stop, test_num, scales);
}
void test_dynamicDE_truthMAC(const char* PATH, double ratio_base, double ratio_step, double ratio_stop, int test_num, vector<uint32_t> scales)
{
	std::cout << " DE static Benchmark file=" << PATH << std::endl << std::endl;
	count_type cnt;
	vector<MAC_ADD> items = read_mac(PATH, cnt);
	std::cout << "The number of items:" << cnt << std::endl;
	vector<pair<MAC_ADD, bool>> dataset;
	for (auto& mac : items)
	{
		dataset.push_back(make_pair(mac, hash32(mac, 2) & 1));
	}
	test_dynamicDE(dataset, "truth", ratio_base, ratio_step, ratio_stop, test_num, scales);
}
void test_dynamicDE_randomMAC(int N, double ratio_base, double ratio_step, double ratio_stop, int test_num, vector<uint32_t> scales)
{
	std::cout << " DE static Benchmark random" << std::endl << std::endl;
	vector<MAC_ADD> items = generate_mac(N);
	vector<pair<MAC_ADD, bool>> dataset;
	for (auto& mac : items)
	{
		dataset.push_back(make_pair(mac, hash32(mac, 2) & 1));
	}
	test_dynamicDE(dataset, "generate", ratio_base, ratio_step, ratio_stop, test_num, scales);
}
int main() {
	/*unordered_map<uint32_t, vector<Edge>> mp;
	mp[1] = { Edge(1,2,1),Edge(1,5,1),Edge(1,6,1) };
	mp[2] = { Edge(1,2,1),Edge(2,3,1),Edge(2,5,1) };
	mp[3] = { Edge(2,3,1),Edge(3,4,1) };
	mp[4] = { Edge(3,4,1),Edge(4,5,1) };
	mp[5] = { Edge(1,5,1),Edge(2,5,1),Edge(4,5,1) };
	mp[6] = { Edge(1,6,1),Edge(6,7,1),Edge(6,9,1),Edge(6,8,1) };
	mp[7] = { Edge(6,7,1) };
	mp[8] = { Edge(6,8,1),Edge(8,9,1) };
	mp[9] = { Edge(6,9,1),Edge(8,9,1) };
	unordered_map<uint32_t, uint32_t> dfn, low;
	stack<pair<unordered_map<uint32_t, vector<Edge>>, vector<uint32_t>>> components;
	tarjan(1, -1, dfn, low, mp, components);
	int t = 1;*/
	test_staticDE_truthMAC((folder[0] + filenames[0]).c_str(), 0.8, 0.02, 1.11, 1000, { 5000,10000,15000 });
	test_dynamicDE_truthMAC((folder[0] + filenames[0]).c_str(), 0.8, 0.01, 1.11, 1000, { 5000,10000,15000 });
	test_staticDE_randomMAC(10000000, 0.8, 0.02, 1.11, 1000, { 10000,100000,1000000 });
	test_dynamicDE_randomMAC(10000000, 0.8, 0.01, 1.11, 100, { 10000,100000,1000000 });
}