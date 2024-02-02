#include <iostream>
#include "common/benchmark.h"
#include"UnifiedTable/DiamondEmbedding.h"
#include"UnifiedTable/ACL.h"
#include"UnifiedTable/FIB.h"
#include"UnifiedTable/MAC.h"
#include"benchmark/CuckooHash.h"
#include"benchmark/NormalHash.h"
#include <chrono>
using namespace std;
const string folder[] = { "../../../datasets/MAC/" ,"../../../datasets/CAIDA/", };
const string filenames[] = { "complete_mac_table.txt" ,"trace3.dat","130000.dat" };
void test(vector<pair<MAC_ADD, PORT>>& dataset, uint32_t memory, int test_num,
	int mac_max_err, int cc_max_err, int hsh_max_err,
	double& mac_score, double& cc_score, double& hsh_score)
{
	MAC* mac;
	CuckooHash<MAC_ADD, PORT>* cc;
	NormalHash<MAC_ADD, PORT>* hsh;
	int cnt = 0;
	for (size_t round = 0; cnt < test_num; round++)
	{
		random_shuffle(dataset.begin(), dataset.end());
		int prelinmary = memory * 8 / log2(1 + MAX_MAC_PORT) / 2.0 * 0.8, result = 0;
		mac = new MAC(vector<pair<MAC_ADD, PORT>>(dataset.begin(), dataset.begin() + prelinmary), result, 2.0);
		if (result == -1)
		{
			delete mac;
			continue;
		}
		int mac_err = mac_max_err, cc_err = cc_max_err, hsh_err = hsh_max_err;
		++cnt;
		uint32_t i;
		for (i = prelinmary; i < dataset.size(); i++)
		{
			if (mac->insert(dataset[i]) == -1)
			{
				if (mac_err == 0)
				{
					mac_score += i;
					break;
				}
				mac_err--;
			}
		}
		if (i == dataset.size())
		{
			mac_score += dataset.size();
		}
		delete mac;
		cc = new CuckooHash<MAC_ADD, PORT>(memory, log2(1 + MAX_MAC_PORT));
		for (i = 0; i < dataset.size(); i++)
		{
			if (cc->insert(dataset[i]) == -1)
			{
				if (cc_err == 0)
				{
					cc_score += i;
					break;
				}
				cc_err--;
			}
		}
		if (i == dataset.size())
		{
			cc_score += dataset.size();
		}
		delete cc;
		hsh = new NormalHash<MAC_ADD, PORT>(memory, log2(1 + MAX_MAC_PORT));
		for (i = 0; i < dataset.size(); i++)
		{
			if (hsh->insert(dataset[i]) == -1)
			{
				if (hsh_err == 0)
				{
					hsh_score += i;
					break;
				}
				hsh_err--;
			}
		}
		if (i == dataset.size())
		{
			hsh_score += dataset.size();
		}
		delete hsh;
	}
	mac_score /= test_num;
	cc_score /= test_num;
	hsh_score /= test_num;
}
void test_MAC_memory(const char* PATH, vector<uint32_t> memorys, int test_num)
{
	uint32_t cnt = 0;
	vector<pair<MAC_ADD, PORT>> dataset = read_mac_port(PATH, cnt);
	ofstream out1, out2;
	out1.open("../result/MAC_memory_Entry.csv");
	out1 << "MEM(KB)" << "," << "UnifiedTable" << "," << "CuckooFilter" << "," << "Hash\n";
	MAC* mac;
	CuckooHash<MAC_ADD, PORT>* cc;
	NormalHash<MAC_ADD, PORT>* hsh;
	for (auto& memory : memorys)
	{
		out1 << memory / 1000 << ",";
		double mac_score = 0, cc_score = 0, hsh_score = 0;
		int cnt = 0;
		test(dataset, memory, test_num, 0, 1, 1, mac_score, cc_score, hsh_score);
		out1 << mac_score << "," << cc_score << "," << hsh_score << endl;
		cout << "MEM=" << memory << "B" << " UnifiedTable=" << mac_score << " Cuckoo=" << cc_score << " hsh=" << hsh_score << endl;
	}
	cout << "SUCCESS\n";
}
void test_MAC_times(const char* PATH, vector<uint32_t> times, uint32_t memory, int test_num)
{
	uint32_t cnt = 0;
	vector<pair<MAC_ADD, PORT>> dataset = read_mac_port(PATH, cnt);
	ofstream out1, out2;
	out1.open("../result/MAC_times_Entry.csv");
	out1 << "MEM(KB)" << "," << "UnifiedTable" << "," << "CuckooFilter" << "," << "Hash\n";
	MAC* mac;
	CuckooHash<MAC_ADD, PORT>* cc;
	NormalHash<MAC_ADD, PORT>* hsh;
	for (auto& time : times)
	{
		out1 << time << ",";
		double mac_score = 0, cc_score = 0, hsh_score = 0;
		test(dataset, memory, test_num, time - 1, time, time, mac_score, cc_score, hsh_score);
		out1 << mac_score << "," << cc_score << "," << hsh_score << endl;
		cout << "ErrTimes=" << time << "" << " UnifiedTable=" << mac_score << " Cuckoo=" << cc_score << " hsh=" << hsh_score << endl;
	}
	cout << "SUCCESS\n";
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
	//test_staticDE_truthMAC((folder[0] + filenames[0]).c_str(), 0.8, 0.02, 1.11, 1000, { 5000,10000,15000 });
	//test_dynamicDE_truthMAC((folder[0] + filenames[0]).c_str(), 0.8, 0.01, 1.11, 1000, { 5000,10000,15000 });
	//test_staticDE_randomMAC(10000000, 0.8, 0.02, 1.11, 1000, { 10000,100000,1000000 });
	//test_dynamicDE_randomMAC(10000000, 0.8, 0.01, 1.11, 100, { 10000,100000,1000000 });
	//test_DE_CAIDA((folder[1] + filenames[2]).c_str());
	//test_MAC();
	//test_FIB((folder + filenames[0]).c_str());
	test_MAC_memory((folder[0] + filenames[0]).c_str(), vector<uint32_t>{1000, 2000, 4000, 8000, 16000}, 1000);
	test_MAC_times((folder[0] + filenames[0]).c_str(), vector<uint32_t>{1, 4, 16, 64}, 4000, 1000);
}