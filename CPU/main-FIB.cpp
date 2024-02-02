#include <iostream>
#include "common/benchmark.h"
#include"UnifiedTable/DiamondEmbedding.h"
#include"UnifiedTable/ACL.h"
#include"UnifiedTable/FIB.h"
#include"UnifiedTable/MAC.h"
#include"UnifiedTable/ARP.h"
#include "benchmark/SAIL.h"
#include"benchmark/CuckooHash.h"
#include"benchmark/NormalHash.h"
#include <chrono>
using namespace std;
vector<string> folder = { "../../../datasets/MAC/" ,"../../../datasets/CAIDA/" };
vector<string> filenames = { "complete_mac_table.txt" ,"trace3.dat","130000.dat","complete_arp_table.txt","rib.txt" ,"acl10K.txt" };
vector<pair<string, string>> filenames1 = {
	make_pair("FIB_20190101_32709.txt","2019"),
	make_pair("FIB_20200101_32709.txt","2020"),
	make_pair("FIB_20210101_32709.txt","2021"),
	make_pair("FIB_20220101_32709.txt","2022"),
	make_pair("FIB_20230101_32709.txt","2023"),
	make_pair("FIB_20240101_32709.txt","2024")
};
vector<pair<string, string>> filenames2 = {
	make_pair("FIB_20200101_14630.txt","FIB1"),
	make_pair("FIB_20200101_19016.txt","FIB2"),
	make_pair("FIB_20200101_19151.txt","FIB3"),
	make_pair("FIB_20200101_23367.txt","FIB4"),
	make_pair("FIB_20200101_32709.txt","FIB5"),
};
uint32_t sum1 = 0;
void test_FIB(const char* root_PATH, const vector<pair<string, string>>& filenames, string prefix)
{
	ofstream out1, out2;
	out1.open("../result/FIB_" + prefix + "_Memory(KB).csv");
	out1 << "MEM(KB)" << "," << "UnifiedTable" << "," << "SAIL" << "," << "PBF" << "\n";
	int cnt = 0;
	for (auto& p : filenames)
	{
		string file = p.first;
		string type = p.second;
		string PATH = root_PATH + file;
		std::cout << "FIB Benchmark file=" << PATH << std::endl;
		uint32_t cnt = 0;
		auto dataset = read_fib(PATH.c_str(), cnt);
		cout << "Toal " << cnt << " items" << endl;
		int result = 0;
		FIB* fib = new FIB(dataset, result);
		if (result == -1)
		{
			cout << "Down!" << endl;
			return;
		}
		SAIL* sail = new SAIL(dataset);
		uint32_t PBF = 18.75 * dataset.size() / 8;
		cout << "OnChipMemoryCost UnifiedTable=" << fib->get_onchip_memory() / 1000 << "KB"
			<< " SAIL=" << sail->get_onchip_memory() / 1000 << "KB"
			<< " PBF=" << PBF / 1000 << "KB" << endl << endl;
		out1 << type << "," << fib->get_onchip_memory() << "," << sail->get_onchip_memory() << "," << PBF << endl;
		delete fib;
		delete sail;
	}
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
	uint32_t cnt = 0;
	//auto dataset = read_fib((folder[0] + filenames[3]).c_str(), cnt);
	test_FIB((folder[0] + "ChicagoFIBDataset/byYear/").c_str(), filenames1, "Year");
	test_FIB((folder[0] + "ChicagoFIBDataset/byID/").c_str(), filenames2, "FIB");
}