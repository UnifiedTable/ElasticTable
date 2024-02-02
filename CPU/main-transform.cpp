#include <iostream>
#include "common/benchmark.h"
#include"UnifiedTable/DiamondEmbedding.h"
#include"UnifiedTable/ACL.h"
#include"UnifiedTable/FIB.h"
#include"UnifiedTable/MAC.h"
#include"UnifiedTable/ARP.h"
#include"common/cmdline.h"
#include <chrono>
enum DATASET_TYPE
{
	D_MAC,
	D_ARP,
	D_FIB,
	D_ACL
};
using namespace std;
uint32_t sum1 = 0;
int main(int argc, char* argv[])
{
	cmdline::parser argparse;
	argparse.add<uint32_t>("type", 't', "dataset type\n\t0 : MAC\n\t1 : ARP\n\t2 : FIB\n\t3 : ACL", true);
	argparse.add<string>("dataset", 'd', "dataset path", true);
	argparse.add<string>("output", 'o', "output path", false, "result.txt");
	argparse.add<bool>("bipartite", 'b', "need bipartite mode", false, false);
	argparse.add<double>("ratio", 'r', "diamond ratio", false, 1.95);
	argparse.parse_check(argc, argv);
	ofstream out(argparse.get<string>("output"));
	uint32_t cnt = 0;
	int result = 0;
	switch (argparse.get<uint32_t>("type"))
	{
	case D_MAC:
	{
		vector<pair<MAC_ADD, PORT>> data = read_mac_port(argparse.get<string>("dataset").c_str(), cnt);
		MAC table(data, result, argparse.get<double>("ratio"), argparse.get<bool>("bipartite"));
		if (result == 0)
		{
			cout << " Building SUCCESS\n";
			out << table << endl;
		}
		else
		{
			cout << " Building FAIL\n";
			exit(-1);
		}
		break;
	}
	case D_ARP:
	{
		vector<pair<IP, MAC_ADD>> data = read_arp(argparse.get<string>("dataset").c_str(), cnt);
		ARP table(data, result, argparse.get<double>("ratio"), argparse.get<bool>("bipartite"));
		if (result == 0)
		{
			cout << " Building SUCCESS\n";
			out << table << endl;
		}
		else
		{
			cout << " Building FAIL\n";
			exit(-1);
		}
		break;
	}
	case D_FIB:
	{
		vector<pair<IP, HOP>> data = read_fib(argparse.get<string>("dataset").c_str(), cnt);
		FIB table(data, result, argparse.get<double>("ratio"), argparse.get<bool>("bipartite"));
		if (result == 0)
		{
			cout << " Building SUCCESS\n";
			out << table << endl;
		}
		else
		{
			cout << " Building FAIL\n";
			exit(-1);
		}
		break;
	}
	case D_ACL:
	{
		vector<pair<ACL_RULE, ACL_ACTION>> data = read_acl(argparse.get<string>("dataset").c_str(), cnt);
		ACL table(data, result, argparse.get<double>("ratio"), argparse.get<bool>("bipartite"));
		if (result == 0)
		{
			cout << " Building SUCCESS\n";
			out << table << endl;
		}
		else
		{
			cout << " Building FAIL\n";
			exit(-1);
		}
		break;
	}
	default:
	{
		cout << "wrong data type\n";
		break;
	}
	}
	out.close();
}
/*#include<iostream>
#include<sstream>//Í·ÎÄ¼þ
#include<fstream>
#include"common/cmdline.h"
using namespace std;
enum DATASET_TYPE
{
	D_MAC,
	D_ARP,
	D_FIB,
	D_ACL
};
template<typename T>
void dg(T& x, istringstream& is)
{
	is >> x;
}
int main(int argc, char* argv[])
{
	cmdline::parser argparse;
	argparse.add<DATASET_TYPE>("type", 't', "dataset type", true);
	argparse.add<string>("dataset", 'd', "dataset path", true);
	argparse.add<string>("output", 'o', "output path", false, "result.txt");
	argparse.add<bool>("bipartite", 'b', "need bipartite mode", false, false);
	argparse.add<double>("ratio", 'd', "diamond ratio", false, 1.92);
	argparse.parse_check(argc, argv);
	return 0;
}*/