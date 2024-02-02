#ifndef FIB_H
#define FIB_H
#include "UnifiedTable.h"
class FIB :public UnifiedTable<IP, HOP>
{
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
			hop = x->hop;
			valid = x->valid;
		}
		TrieNode* left;
		TrieNode* right;
		HOP hop; //从根到当前节点最近的hop
		bool valid; //hop是否真的是他自己的（push出来的不算）
	};
public:
	FIB(vector<pair<IP, HOP>>_table, int& result, double _diamond_ratio = 1.92, uint32_t _bipartite = 0) :UnifiedTable<IP, HOP>(_diamond_ratio)
	{
		build_trie(_table);
		des.resize(IP_LEN);
		vector<vector<pair<IP, bool>>> layer_table(IP_LEN + 1);

		//expand to real trie
		queue<pair<TrieNode*, IP>> v;
		IP ip;
		v.push(make_pair(trie_root, ip));
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
				process_table[ip] = x->hop;
			}
		}
		result = 0;
		for (size_t i = 0; i < IP_LEN; i++)
		{
			if (i == 19)
			{
				int t = 1;
			}
			des[i] = new DiamondEmbedding<IP>(calc_de_memory(diamond_ratio, layer_table[i].size()), _bipartite);
			if (des[i]->build(layer_table[i]) != 0)
			{
				cout << "Down in Layer " << i << " Size=" << layer_table[i].size() << endl;
				result = -1;
			}
		}
	};
	~FIB() {
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
	// 0success -1fail
	virtual int insert(pair<IP, HOP> item)
	{
		TrieNode* x = trie_root;
		pair<TrieNode*, IP> reverse_node;
		bool SUCCESS = true;
		if (add_one_item(item, reverse_node))
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
					process_table[ip] = x->hop;
				}
			}
		}
		else
		{
			//结束在老节点，对trie结构以及DE无影响，更新子树hop
			update_subtree_hop(reverse_node);
		}
		if (SUCCESS == true) return 0;
		else return -1;
	}
	virtual void withdraw(pair<IP, HOP> item)
	{
		IP ip = item.first, now_ip;
		HOP hop = item.second;
		TrieNode* ptr = trie_root, * parent = NULL;
		stack<pair<TrieNode*, IP>> pass;
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
		ptr->valid = false;
		push(parent);
		if (ptr->left == NULL)
		{
			// 叶子节点，视情况对trie进行剪枝
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
			update_subtree_hop(make_pair(ptr, ip));
		}
	}
	virtual HOP query(IP item) const
	{
		for (size_t i = 0; i < IP_LEN; i++)
		{
			IP ip = item.mask(i);
			if (des[i]->query(ip) == 1)
			{
				HOP hop;
				exact_match(ip, hop);
				return hop;
			}
		}
		HOP hop;
		exact_match(item, hop);
		return hop;
	}
	bool exact_match(IP item, HOP& ret) const
	{
		if (process_table.find(item) != process_table.end())
		{
			ret = process_table.find(item)->second;
			return true;
		}
		return false;
	}
	virtual uint32_t get_onchip_memory() const
	{
		uint32_t sum = 0;
		for (size_t i = 0; i < IP_LEN; i++)
		{
			sum += des[i]->get_onchip_memory();
		}
		return sum;
	}
	virtual string get_info() const
	{
		stringstream out;
		out << "FIB TABLE INFO\n";
		for (size_t i = 0; i < IP_LEN; i++)
		{
			out << "LAYER " << i << " " << *des[i] << endl;
		}
		return out.str();
	}
private:
	void update_subtree_hop(pair<TrieNode*, IP> root)
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
				process_table[ip] = x->hop;
			}
		}
	}
	//在真二叉树下返回第一个遇到的从叶子节点变中间节点的节点，若没有，则reverse_node为最后一个节点
	bool add_one_item(pair<IP, HOP> item, pair<TrieNode*, IP>& reverse_nodes)
	{
		IP ip = item.first, now_ip;
		HOP hop = item.second;
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
		ptr->hop = hop;
		ptr->valid = true;
		if (!create)
		{
			reverse_nodes = make_pair(ptr, ip);
		}
		return create;
	}
	void build_trie(const vector<pair<IP, HOP>>& table)
	{
		trie_root = new TrieNode();
		for (auto& item : table)
		{
			pair<TrieNode*, IP> reverse_node;
			add_one_item(item, reverse_node);
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
			node->left->hop = node->hop;
		}
		if (node->right == NULL)
		{
			auto new_node = new TrieNode();
			node->right = new_node;
		}
		if (!node->right->valid)
		{
			node->right->hop = node->hop;
		}
	}
	TrieNode* trie_root;
	unordered_map<IP, HOP> process_table;
};
class SlightFIB :SlightUnifiedTable<IP, HOP>
{
public:
	SlightFIB(FIB* mather_table) :SlightUnifiedTable<IP, HOP>(mather_table) {}
	//LPM
	virtual HOP query(IP item) const {
		for (size_t i = 0; i < IP_LEN; i++)
		{
			IP ip = item.mask(i);
			if (des[i]->query(ip) == 1)
			{
				HOP hop;
				((FIB*)off_chip)->exact_match(ip, hop);
				return hop;
			}
		}
		HOP hop;
		((FIB*)off_chip)->exact_match(item, hop);
		return hop;
	}
	virtual uint32_t get_onchip_memory() const
	{
		uint32_t sum = 0;
		for (size_t i = 0; i < IP_LEN; i++)
		{
			sum += des[i]->get_onchip_memory();
		}
		return sum;
	}
private:
};

#endif // !FIB_H
