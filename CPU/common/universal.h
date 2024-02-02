#ifndef UNIVERSAL_H
#define UNIVERSAL_H
#include "hash.h"
#include <unordered_map>
#include<sstream>
#define MAX_MAC_PORT 63
#define MAX_HOPS 65535
#define GETBYTE(range) ((range & 0xFF000000) ? 4 : ((range & 0xFF0000) ? 3 : ((range & 0xFF00) ? 2 : 1)))
#define MAC_ADD_LEN 48
#define IP_LEN 32
#define IP_HEADER_LEN 13 * 7
#define MAX_IP_PORT 65535
typedef uint32_t ACL_ACTION;
struct MAC_ADD {
	uint8_t array[(MAC_ADD_LEN + 7) >> 3];
	static uint32_t getLEN() { return MAC_ADD_LEN; }
	bool operator == (const MAC_ADD& b)const {
		return memcmp(array, b.array, sizeof(MAC_ADD)) == 0;
	}
};
struct IP
{
	uint8_t array[(IP_LEN + 7) >> 3];
	uint8_t prefix;
	IP()
	{
		prefix = 0;
		memset(array, 0, (IP_LEN + 7) >> 3);
	}
	void set(int idx, int val)
	{
		WRITE8(array, 1, idx, val);
	}
	uint8_t get(int idx) const
	{
		return ACCESS8(array, 1, idx);
	}
	void add(int val)
	{
		WRITE8(array, 1, prefix, val);
		prefix++;
	}
	void dec()
	{
		prefix--;
		WRITE8(array, 1, prefix, 0);
	}
	IP mask(int new_prefix) const
	{
		IP ret;
		ret.prefix = new_prefix;
		copy(array, array + ((IP_LEN + 7) >> 3), ret.array);
		for (size_t i = (new_prefix >> 3) + 1; i < ((IP_LEN + 7) >> 3); i++)
		{
			ret.array[i] = 0;
		}
		if (new_prefix != 32)
			ret.array[new_prefix >> 3] &= ((1 << (new_prefix & 7)) - 1) << (8 - new_prefix & 7);
		return ret;
	}
	void locate_prefix()
	{
		prefix = 0;
		for (int i = IP_LEN; i >= 0; i--)
		{
			if (ACCESS8(array, 1, i))
			{
				prefix = i + 1;
				break;
			}
		}
	}
	bool operator == (const IP& b) const {
		return memcmp(array, b.array, IP_LEN >> 3) == 0 && prefix == b.prefix;
	}
};
struct IP_HEADER {
	//srcip(4)+dstip(4)+srcport(2)+dstport(2)+protocol(1)
	uint8_t array[IP_HEADER_LEN];
	IP get_srcIP() const
	{
		IP ret;
		copy(array, array + (IP_LEN >> 3), ret.array);
		ret.prefix = IP_LEN;
		return ret;
	}
	IP get_dstIP() const
	{
		IP ret;
		copy(array + (IP_LEN >> 3), array + (IP_LEN >> 2), ret.array);
		ret.prefix = IP_LEN;
		return ret;
	}
	uint16_t get_srcPort() const
	{
		return *(uint16_t*)(array + 8);
	}
	uint16_t get_dstPort() const
	{
		return *(uint16_t*)(array + 10);
	}
	uint8_t get_protocol() const
	{
		return array[12];
	}
	bool operator == (const IP_HEADER& b)const {
		return memcmp(array, b.array, sizeof(IP_HEADER)) == 0;
	}
};
struct ACL_RULE {
	IP srcIP, dstIP;
	uint16_t srcPortStart, srcPortStop; // 查询结构体下port放在start位置
	uint16_t dstPortStart, dstPortStop;
	bool operator == (const ACL_RULE& b)const {
		return srcIP == b.srcIP
			&& dstIP == b.dstIP
			&& srcPortStart == b.srcPortStart && srcPortStop == b.srcPortStop
			&& dstPortStart == b.dstPortStart && dstPortStop == b.dstPortStop;
	}
};
struct PORT {
	uint8_t array[GETBYTE(MAX_MAC_PORT)];
	bool operator == (const PORT& b)const {
		return memcmp(array, b.array, sizeof(PORT)) == 0;
	}
};
struct HOP {
	uint8_t array[GETBYTE(MAX_HOPS)];
	HOP()
	{
		memset(array, 0, GETBYTE(MAX_HOPS));
	}
	bool operator == (const HOP& b)const {
		return memcmp(array, b.array, sizeof(HOP)) == 0;
	}
};
namespace std {
	template<>
	struct hash<IP> {
		size_t operator()(const IP& item) const noexcept
		{
			return Hash::BOBHash32((uint8_t*)&item, sizeof(IP), 0);
		}
	};
}
uint32_t hash32(const MAC_ADD& item, uint32_t seed = 0) {
	int t = sizeof(MAC_ADD);
	return Hash::BOBHash32((uint8_t*)&item, sizeof(MAC_ADD), seed);
}
MAC_ADD read_mac(string s)
{
	MAC_ADD mac;
	char* str; int j = 0;
	for (size_t i = 0; i < (MAC_ADD_LEN >> 2); i++)
	{
		if (s[j] == '.')
		{
			j++;
		}
		char ch = s[j];
		uint32_t segment = strtol(&ch, &str, 16);
		WRITE8(mac.array, 4, i, segment);
		j++;
	}
	return mac;
}
IP read_IP(string s)
{
	IP ip;
	s += ' ';
	char* s1 = (char*)s.c_str();
	for (size_t i = 0; i < (IP_LEN >> 3); i++)
	{
		uint32_t segment = strtol(s1, &s1, 10);
		s1++;
		WRITE8(ip.array, 8, i, segment);
	}
	if (*s1 >= 48 && *s1 <= 57)
	{
		ip.prefix = strtol(s1, &s1, 10);
	}
	else
	{
		ip.prefix = IP_LEN;
	}
	if (ip.prefix > 32)
	{
		int t = 1;
	}
	ip = ip.mask(ip.prefix);
	return ip;
}
HOP read_hop(string s1)
{
	HOP hop;
	char* s = (char*)s1.c_str();
	uint32_t segment = strtol(s, &s, 10);
	int t = GETBYTE(MAX_HOPS);
	for (size_t i = 0; i < t; i++)
	{
		WRITE8(hop.array, 8, i, segment & 0xff);
		segment >>= 8;
	}
	return hop;
}
vector<MAC_ADD> read_mac(const char* PATH, count_type& cnt)
{
	cnt = 0;
	vector<MAC_ADD> dataset;
	ifstream fin(PATH);
	string line;
	while (getline(fin, line))
	{
		MAC_ADD mac = read_mac(line);
		dataset.push_back(mac);
		cnt++;
	}
	return dataset;
}
vector<MAC_ADD> generate_mac(uint32_t N)
{
	vector<MAC_ADD> items;
	for (size_t i = 0; i < N; i++)
	{
		MAC_ADD mac;
		for (size_t j = 0; j < (MAC_ADD_LEN >> 3); j++)
		{
			mac.array[j] = rand() & 0xff;
		}
		items.push_back(mac);
	}
	return items;
}
vector<pair<IP, MAC_ADD>> read_arp(const char* PATH, uint32_t& cnt)
{
	cnt = 0;
	vector<pair<IP, MAC_ADD>> dataset;
	ifstream fin(PATH);
	string line, element;
	while (getline(fin, line))
	{
		stringstream ss(line);
		MAC_ADD mac;
		IP ip;
		int cnt1 = 0;
		while (ss >> element)
		{
			cnt1++;
			if (cnt1 == 2)
			{
				mac = read_mac(element);
			}
			else
			{
				ip = read_IP(element);
			}
		}
		cnt++;
		dataset.push_back(make_pair(ip, mac));
	}
	return dataset;
}
vector<pair<MAC_ADD, PORT>> read_mac_port(const char* PATH, uint32_t& cnt)
{
	cnt = 0;
	vector<pair<MAC_ADD, PORT>> dataset;
	unordered_map<string, PORT> mp;
	ifstream fin(PATH);
	string line, element;
	while (getline(fin, line))
	{
		stringstream ss(line);
		MAC_ADD mac;
		int cnt1 = 0;
		while (ss >> element)
		{
			cnt1++;
			if (element.size() == 14)
			{
				mac = read_mac(element);
			}
		}
		string temp = element.substr(0, element.find(','));
		if (cnt1 == 3)
		{
			if (mp.find(temp) == mp.end())
			{
				PORT port;
				int temp1 = cnt++, t = GETBYTE(MAX_MAC_PORT);
				for (size_t i = 0; i < t; i++)
				{
					WRITE8(port.array, 8, i, temp1 & 0xff);
					temp1 >>= 8;
				}
				mp[temp] = port;
			}
			cnt++;
			dataset.push_back(make_pair(mac, mp[temp]));
		}
	}
	return dataset;
}
vector<pair<IP, HOP>> read_fib(const char* PATH, uint32_t& cnt)
{
	cnt = 0;
	vector<pair<IP, HOP>> dataset;
	ifstream fin(PATH);
	string line, element;
	while (getline(fin, line))
	{
		stringstream ss(line);
		IP ip;
		HOP hop;
		int cnt1 = 0;
		while (ss >> element)
		{
			cnt1++;
			if (cnt1 == 1)
			{
				ip = read_IP(element);
				if (ip.prefix == 0)
				{
					int t = 1;
				}
			}
			else
			{
				hop = read_hop(element);
			}
		}
		++cnt;
		dataset.push_back(make_pair(ip, hop));
	}
	return dataset;
}
vector<pair<ACL_RULE, ACL_ACTION>> read_acl(const char* PATH, uint32_t& cnt)
{
	cnt = 0;
	vector<pair<ACL_RULE, ACL_ACTION>> dataset;
	ifstream fin(PATH);
	string line, element;
	while (getline(fin, line))
	{
		stringstream ss(line.substr(1));
		ACL_RULE acl;
		IP ip;
		ACL_ACTION action;
		int cnt1 = 0;
		while (ss >> element)
		{
			cnt1++;
			if (cnt1 <= 2)
			{
				ip = read_IP(element);
				if (cnt1 == 1) acl.srcIP = ip;
				else acl.dstIP = ip;
			}
			else if (cnt1 <= 4)
			{
				acl.srcPortStart = stoi(element);
				ss >> element >> element;
				acl.srcPortStop = stoi(element);
				ss >> element;
				acl.dstPortStart = stoi(element);
				ss >> element >> element;
				acl.dstPortStop = stoi(element);
				ss >> element;
				cnt1 += 2;
			}
			else
			{
				char* s = (char*)element.c_str();
				uint32_t segment1 = strtol(s, &s, 16);
				s++;
				uint32_t segment2 = strtol(s, &s, 16);
				action = (segment1 << 16) + segment2;
			}
		}
		++cnt;
		dataset.push_back(make_pair(acl, action));
	}
	return dataset;
}
uint32_t calc_de_memory(double diamond_ratio, uint32_t siz)
{
	if (siz < 500)
	{
		return 500;
	}
	if (siz < 1000)
	{
		return ceil(4 * siz / 8.0);
	}
	else if (siz < 5000)
	{
		return ceil(2.5 * siz / 8.0);
	}
	else if (siz < 50000)
	{
		return ceil(2.25 * siz / 8.0);
	}
	else if (siz < 100000)
	{
		return ceil(2.0 * siz / 8.0);
	}
	else
	{
		return ceil(diamond_ratio * siz / 8.0);
	}
}

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
void getFileNames(std::string path, std::vector<std::string>& files)
{
	DIR* pDir;
	struct dirent* ptr;
	if (!(pDir = opendir(path.c_str()))) {
		std::cout << "Folder doesn't Exist!" << std::endl;
		exit(-1);
	}
	while ((ptr = readdir(pDir)) != 0) {
		if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
			std::string filename(ptr->d_name);
			files.push_back(path + "/" + filename);
		}
	}
}
#else
#include<io.h>
void getFileNames(std::string path, std::vector<std::string>& files)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,递归查找
			//如果不是,把文件绝对路径存入vector中
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFileNames(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
#endif // WIN32

#endif // !UNIVERSAL_H
