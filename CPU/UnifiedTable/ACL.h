#pragma once
#ifndef ACL_H
#define ACL_H
#include"UnifiedTable.h"
class ACL :public UnifiedTable<ACL_RULE, ACL_ACTION>
{
	typedef vector<uint32_t> INDEX_LIST;
	struct TrieNode
	{
		TrieNode() {
			left = right = NULL;
			valid = false;
		}
		TrieNode(TrieNode* x)
		{
			left = x->left;
			right = x->right;
			index_list = x->index_list;
			valid = x->valid;
		}
		TrieNode* left;
		TrieNode* right;
		INDEX_LIST index_list; //从根到当前节点最近的
		bool valid; //index_list是否真的是他自己的（push出来的不算）
	};
public:
	ACL(vector<pair<ACL_RULE, ACL_ACTION>> _table, int& result, vector<double>& stat, double _diamond_ratio = 1.92, uint32_t _bipartite = 0) : table(_table), UnifiedTable<ACL_RULE, ACL_ACTION>(_diamond_ratio)
	{
		unordered_map<IP, INDEX_LIST> src_table, dst_table;
		uint32_t index = 0;
		src_port_table.resize(MAX_IP_PORT + 1);
		dst_port_table.resize(MAX_IP_PORT + 1);
		valid_table.resize(table.size(), true);
		for (auto& item : _table)
		{
			ACL_ACTION act = item.second;
			IP src_ip = item.first.srcIP, dst_ip = item.first.dstIP;
			src_table[src_ip].push_back(index);
			dst_table[dst_ip].push_back(index);
			for (size_t port = item.first.srcPortStart; port < item.first.srcPortStop; port++)
			{
				src_port_table[port].push_back(index);
			}
			for (size_t port = item.first.dstPortStart; port < item.first.dstPortStop; port++)
			{
				dst_port_table[port].push_back(index);
			}
			index++;
		}
		int result0 = 0, result1 = 0;
		cout << "Building src table\n";
		stat[0] = build_diamond_embedding(src_table, result0, src_ip_des, src_trie_root, src_process_table, _bipartite);
		stat[1] = src_process_table.size();
		cout << "Building dst table\n";
		stat[2] = build_diamond_embedding(dst_table, result1, dst_ip_des, dst_trie_root, dst_process_table, _bipartite);
		stat[3] = dst_process_table.size();
		result = min(result0, result1);
	}
	~ACL() {
		clear(src_trie_root);
		clear(dst_trie_root);
	}
	// 0success -1fail
	virtual int insert(pair<ACL_RULE, ACL_ACTION> item)
	{
		table.push_back(item);
		valid_table.push_back(true);
		IP src_ip = item.first.srcIP, dst_ip = item.first.dstIP;
		int result = 0;
		result = min(result, insert(src_trie_root, make_pair(src_ip, item.second), src_process_table, src_ip_des));
		result = min(result, insert(dst_trie_root, make_pair(src_ip, item.second), dst_process_table, dst_ip_des));
		if (result == -1) return -1;
		uint32_t index = table.size() - 1;
		for (size_t port = item.first.srcPortStart; port < item.first.srcPortStop; port++)
		{
			src_port_table[port].push_back(index);
		}
		for (size_t port = item.first.dstPortStart; port < item.first.dstPortStop; port++)
		{
			dst_port_table[port].push_back(index);
		}
		return 0;
	}
	virtual void withdraw(pair<ACL_RULE, ACL_ACTION> item)
	{
		uint32_t index = 0;
		for (size_t i = 0; i < table.size(); i++)
		{
			if (table[i] == item)
			{
				index = i;
				valid_table[index] = false;
				break;
			}
		}
		IP src_ip = item.first.srcIP, dst_ip = item.first.dstIP;
		withdraw(src_trie_root, make_pair(src_ip, index), src_process_table, src_ip_des);
		withdraw(dst_trie_root, make_pair(dst_ip, index), dst_process_table, dst_ip_des);
		for (size_t port = item.first.srcPortStart; port < item.first.srcPortStop; port++)
		{
			src_port_table[port].erase(find(src_port_table[port].begin(), src_port_table[port].end(), index));
		}
		for (size_t port = item.first.dstPortStart; port < item.first.dstPortStop; port++)
		{
			dst_port_table[port].erase(find(dst_port_table[port].begin(), dst_port_table[port].end(), index));
		}
	}
	virtual ACL_ACTION query(ACL_RULE item) const
	{
		IP src_ip = item.srcIP, dst_ip = item.dstIP, src, dst;
		uint16_t src_port = item.srcPortStart, dst_port = item.dstPortStart;
		//src_ip
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			IP ip = src_ip.mask(i);
			if (src_process_table.find(ip) != src_process_table.end())
			{
				HOP hop;
				src = ip;
				break;
			}
		}
		//dst_ip
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			IP ip = dst_ip.mask(i);
			if (dst_process_table.find(ip) != dst_process_table.end())
			{
				HOP hop;
				dst = ip;
				break;
			}
		}
		ACL_ACTION ret;
		exact_match(src, dst, src_port, dst_port, ret);
		return ret;
	}
	void get_slight_embedding(vector<SlightDiamondEmbedding<IP>*>& src_sdes, vector<SlightDiamondEmbedding<IP>*>& dst_sdes)
	{
		for (auto& de : src_ip_des)
		{
			src_sdes.push_back(new SlightDiamondEmbedding<IP>(de));
		}
		for (auto& de : dst_ip_des)
		{
			dst_sdes.push_back(new SlightDiamondEmbedding<IP>(de));
		}
	}
	bool exact_match(IP src, IP dst, uint16_t src_port, uint16_t dst_port, ACL_ACTION& ret) const
	{
		if (src_process_table.find(src) == src_process_table.end())
		{
			return false;
		}
		auto a = src_process_table.find(src)->second;
		if (dst_process_table.find(dst) == dst_process_table.end())
		{
			return false;
		}
		auto b = dst_process_table.find(dst)->second;
		auto c = src_port_table[src_port], d = dst_port_table[dst_port];
		uint32_t j = 0, k = 0, l = 0;
		bool SUCCESS = false;
		for (size_t i = 0; i < a.size(); i++)
		{
			while (j < b.size() && b[j] < a[i])j++;
			while (k < c.size() && c[k] < a[i])k++;
			while (l < d.size() && d[l] < a[i])l++;
			if (a[i] == b[j] && a[i] == c[k] && a[i] == d[l])
			{
				ret = table[a[i]].second;
				SUCCESS = true;
				break;
			}
		}
		return SUCCESS;
	}
	virtual uint32_t get_onchip_memory() const
	{
		uint32_t sum = 0;
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			sum += src_ip_des[i]->get_onchip_memory();
			sum += dst_ip_des[i]->get_onchip_memory();
		}
		return sum;
	}
	virtual string get_info() const
	{
		stringstream out;
		out << "ACL TABLE INFO\n";
		out << "SRC_IP\n";
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			out << "LAYER " << i << " " << *src_ip_des[i] << endl;
		}
		out << "DST_IP\n";
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			out << "LAYER " << i << " " << *dst_ip_des[i] << endl;
		}
		return out.str();
	}
private:
	void clear(TrieNode* trie_root)
	{
		queue<TrieNode*> v;
		v.push(trie_root);
		while (!v.empty())
		{
			auto p = v.front();
			TrieNode* x = p;
			v.pop();
			if (x->left != NULL)
			{
				v.push(x->left);
			}
			if (x->right != NULL)
			{
				v.push(x->right);
			}
			delete x;
		}
	}
	void withdraw(TrieNode* trie_root, pair<IP, uint32_t> item, unordered_map<IP, INDEX_LIST>& process_table, vector<DiamondEmbedding<IP>*>& des)
	{
		IP ip = item.first, now_ip;
		uint32_t index = item.second;
		TrieNode* ptr = trie_root, * parent = NULL;
		stack<pair<TrieNode*, IP>> pass;
		HOP hop;
		bool create = false;
		for (size_t i = 0; i < ip.prefix; i++)
		{
			parent = ptr;
			pass.push(make_pair(ptr, now_ip));
			if (ACCESS8(ip.array, 1, i))
			{
				now_ip.add(1);
				ptr = ptr->right;
			}
			else
			{
				now_ip.add(0);
				ptr = ptr->left;
			}
		}
		ptr->index_list.erase(find(ptr->index_list.begin(), ptr->index_list.end(), index));
		ptr->valid = false;
		push(parent);
		if (ptr->left == NULL)
		{
			// 叶子节点，视情况对trie进行剪枝
			process_table[ip].erase(find(process_table[ip].begin(), process_table[ip].end(), index));
			if (!process_table[ip].empty()) return;
			process_table.erase(ip);
			while (true)
			{
				auto p = pass.top();
				TrieNode* x = p.first;
				IP ip = p.second;
				pass.pop();
				if (x->left->left == NULL && x->right->left == NULL && !x->left->valid && !x->right->valid)
				{
					IP next_ip(ip);
					next_ip.add(0);
					des[next_ip.prefix]->withdraw(next_ip, true);
					next_ip = IP(ip);
					next_ip.add(1);
					des[next_ip.prefix]->withdraw(next_ip, true);
					des[ip.prefix]->withdraw(ip, false);
					des[ip.prefix]->insert(ip, true);
					x->left = x->right = NULL;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			// 中间节点，对trie结构无影响，更新子树hop
			update_subtree_list(make_pair(ptr, ip), process_table);
		}
	}
	int insert(TrieNode* trie_root, pair<IP, ACL_ACTION> item, unordered_map<IP, INDEX_LIST>& process_table, vector<DiamondEmbedding<IP>*>& des)
	{
		TrieNode* x = trie_root;
		pair<TrieNode*, IP> reverse_node;
		bool SUCCESS = true;
		if (add_one_item(trie_root, make_pair(item.first, INDEX_LIST{ table.size() }), reverse_node))
		{
			// 插入了新节点，撤回原来叶子节点的DE插入并插入新点
			IP ip = reverse_node.second;
			process_table.erase(ip);
			des[ip.prefix]->withdraw(ip, true);

			queue<pair<TrieNode*, IP>> v;
			v.push(reverse_node);
			while (!v.empty())
			{
				auto p = v.front();
				TrieNode* x = p.first;
				IP ip = p.second;
				uint32_t depth = ip.prefix;
				v.pop();
				push(x);
				if (x->left != NULL)
				{
					SUCCESS &= des[depth]->insert(ip, false) == 0;
					ip.add(0);
					v.push(make_pair(x->left, ip));
					ip.set(ip.prefix - 1, 1);
					v.push(make_pair(x->right, ip));
				}
				else
				{
					SUCCESS &= des[depth]->insert(ip, true) == 0;
					process_table[ip] = x->index_list;
				}
			}
		}
		else
		{
			//结束在老节点，对trie结构以及DE无影响，更新子树hop
			update_subtree_list(reverse_node, process_table);
		}
		if (SUCCESS == true) return 0;
		else return -1;
	}
	void update_subtree_list(pair<TrieNode*, IP> root, unordered_map<IP, INDEX_LIST>& process_table)
	{
		queue<pair<TrieNode*, IP>> v;
		IP ip;
		v.push(root);
		while (!v.empty())
		{
			auto p = v.front();
			TrieNode* x = p.first;
			IP ip = p.second;
			uint32_t depth = ip.prefix;
			v.pop();
			if (x->valid && x != root.first) continue;
			push(x);
			if (x->left != NULL)
			{
				ip.add(0);
				v.push(make_pair(x->left, ip));
				ip.set(ip.prefix - 1, 1);
				v.push(make_pair(x->right, ip));
			}
			else
			{
				process_table[ip] = x->index_list;
			}
		}
	}
	int build_diamond_embedding(const unordered_map<IP, INDEX_LIST>& table, int& result,
		vector<DiamondEmbedding<IP>*>& des, TrieNode*& root,
		unordered_map<IP, INDEX_LIST>& process_table, uint32_t _bipartite)
	{
		build_trie(root, table);
		des.resize(IP_LEN + 1);
		vector<vector<pair<IP, bool>>> layer_table(IP_LEN + 1);

		//expand to real trie
		queue<pair<TrieNode*, IP>> v;
		IP ip;
		v.push(make_pair(root, ip));
		int cnt = 0;
		while (!v.empty())
		{
			auto p = v.front();
			TrieNode* x = p.first;
			IP ip = p.second;
			uint32_t depth = ip.prefix;
			v.pop();
			push(x);
			if (x->left != NULL)
			{
				layer_table[depth].push_back(make_pair(ip, false));
				ip.add(0);
				v.push(make_pair(x->left, ip));
				ip.set(ip.prefix - 1, 1);
				v.push(make_pair(x->right, ip));
			}
			else
			{
				layer_table[depth].push_back(make_pair(ip, true));
				process_table[ip] = x->index_list;
			}
		}
		result = 0;
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			cnt += layer_table[i].size();
			des[i] = new DiamondEmbedding<IP>(calc_de_memory(diamond_ratio, layer_table[i].size()), _bipartite);
			if (des[i]->build(layer_table[i]) != 0)
			{
				cout << "Down in Layer " << i << " Size=" << layer_table[i].size() << endl;
				result = -1;
			}
		}
		return cnt;
	}
	void update_subtree_index(pair<TrieNode*, IP> root, unordered_map<IP, INDEX_LIST>& process_table)
	{
		queue<pair<TrieNode*, IP>> v;
		IP ip;
		v.push(root);
		while (!v.empty())
		{
			auto p = v.front();
			TrieNode* x = p.first;
			IP ip = p.second;
			uint32_t depth = ip.prefix;
			v.pop();
			if (x->valid && x != root.first) continue;
			push(x);
			if (x->left != NULL)
			{
				ip.add(0);
				v.push(make_pair(x->left, ip));
				ip.set(ip.prefix - 1, 1);
				v.push(make_pair(x->right, ip));
			}
			else
			{
				process_table[ip] = x->index_list;
			}
		}
	}
	//在真二叉树下返回第一个遇到的从叶子节点变中间节点的节点，若没有，则reverse_node为最后一个节点
	bool add_one_item(TrieNode* trie_root, pair<IP, INDEX_LIST> item, pair<TrieNode*, IP>& reverse_nodes)
	{
		IP ip = item.first, now_ip;
		INDEX_LIST index_list = item.second;
		reverse_nodes = make_pair((TrieNode*)NULL, IP());
		TrieNode* ptr = trie_root;
		bool create = false;
		for (size_t i = 0; i < ip.prefix; i++)
		{
			if (!create && ptr->left == NULL && ptr->right == NULL)
			{
				create = true;
				reverse_nodes = make_pair(ptr, now_ip);
			}
			if (ACCESS8(ip.array, 1, i))
			{
				now_ip.add(1);
				if (ptr->right == NULL)
				{
					ptr->right = new TrieNode();
				}
				ptr = ptr->right;
			}
			else
			{
				now_ip.add(0);
				if (ptr->left == NULL)
				{
					ptr->left = new TrieNode();
				}
				ptr = ptr->left;
			}
		}
		for (auto& item : index_list)
		{
			ptr->index_list.push_back(item);
		}
		ptr->valid = true;
		if (!create)
		{
			reverse_nodes = make_pair(ptr, ip);
		}
		return create;
	}
	void build_trie(TrieNode*& trie_root, const unordered_map<IP, INDEX_LIST>& table)
	{
		trie_root = new TrieNode();
		for (auto& item : table)
		{
			pair<TrieNode*, IP> reverse_node;
			add_one_item(trie_root, item, reverse_node);
		}
	}
	void push(TrieNode* node)
	{
		if (node == NULL || ((node->left == NULL) && (node->right == NULL))) return;
		if (node->left == NULL)
		{
			auto new_node = new TrieNode();
			node->left = new_node;
		}
		if (!node->left->valid)
		{
			for (auto& item : node->index_list)
			{
				node->left->index_list.push_back(item);
			}
		}
		if (node->right == NULL)
		{
			auto new_node = new TrieNode();
			node->right = new_node;
		}
		if (!node->right->valid)
		{
			for (auto& item : node->index_list)
			{
				node->right->index_list.push_back(item);
			}
		}
	}
	vector<pair<ACL_RULE, ACL_ACTION>> table;
	vector<bool> valid_table;
	vector<DiamondEmbedding<IP>*> src_ip_des, dst_ip_des;
	vector<INDEX_LIST> src_port_table, dst_port_table;
	TrieNode* src_trie_root, * dst_trie_root;
	unordered_map<IP, INDEX_LIST> src_process_table, dst_process_table;
};
class SlightACL :public SlightUnifiedTable<ACL_RULE, ACL_ACTION>
{
public:
	SlightACL(ACL* mather_table) :SlightUnifiedTable<ACL_RULE, ACL_ACTION>(mather_table)
	{
		mather_table->get_slight_embedding(src_ip_des, dst_ip_des);
	}
	//LPM
	virtual ACL_ACTION query(ACL_RULE item) const
	{
		IP src_ip = item.srcIP, dst_ip = item.dstIP, src, dst;
		uint16_t src_port = item.srcPortStart, dst_port = item.dstPortStart;
		//src_ip
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			IP ip = src_ip.mask(i);
			if (src_ip_des[i]->query(ip) == 1)
			{
				HOP hop;
				src = ip;
				break;
			}
		}
		//dst_ip
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			IP ip = dst_ip.mask(i);
			if (dst_ip_des[i]->query(ip) == 1)
			{
				HOP hop;
				dst = ip;
				break;
			}
		}
		ACL_ACTION ret;
		((ACL*)off_chip)->exact_match(src, dst, src_port, dst_port, ret);
		return ret;
	}
	virtual uint32_t get_onchip_memory() const
	{
		uint32_t sum = 0;
		for (size_t i = 0; i <= IP_LEN; i++)
		{
			sum += src_ip_des[i]->get_onchip_memory();
			sum += dst_ip_des[i]->get_onchip_memory();
		}
		return sum;
	}
private:
	vector<SlightDiamondEmbedding<IP>*> src_ip_des, dst_ip_des;
};

#endif // !ACL_H
