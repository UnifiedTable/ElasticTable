#pragma once
#include "../common/universal.h"
class SAIL
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
	SAIL(vector<pair<IP, HOP>>_table)
	{
		build_trie(_table);
		buildBCN();
	}
	~SAIL()
	{
	}
	uint32_t get_onchip_memory() const
	{
		uint32_t sum = BCN16.size() * 2 + BCN24.size() / 8;
		return sum;
	}
	HOP query(IP item) const
	{
		uint32_t ip = *(uint32_t*)item.array;
		uint16_t hop;
		if (BCN16[ip >> 16] & 0x8000)
		{
			hop = BCN16[ip >> 16] & 0x7fff;
		}
		else
		{
			uint32_t index = (BCN16[ip >> 16] & 0x7fff) << 8 + ((ip << 16) >> 24);
			if (BCN24[index] & 0x8000)
			{
				hop = BCN24[index] & 0x7fff;
			}
			else
			{
				index = (BCN24[index] & 0x7fff) << 8 + (ip & 0xffff);
				hop = N32[index];
			}
		}
		return read_hop(to_string(hop));
	}
private:
	void push(TrieNode* node, uint32_t depth, bool expandable)
	{
		if (node == NULL) return;
		if (node->left == NULL && expandable)
		{
			auto new_node = new TrieNode();
			node->left = new_node;
		}
		if (node->left != NULL && !node->left->valid)
		{
			node->left->hop = node->hop;
		}
		if (node->right == NULL && expandable)
		{
			auto new_node = new TrieNode();
			node->right = new_node;
		}
		if (node->right != NULL && !node->right->valid)
		{
			node->right->hop = node->hop;
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
	void buildBCN()
	{
		queue<pair<TrieNode*, pair<uint32_t, bool>>> v;
		v.push(make_pair(trie_root, make_pair(0, true)));
		uint32_t c16 = 0, c24 = 0;
		while (!v.empty())
		{
			auto p = v.front();
			TrieNode* x = p.first;
			uint32_t depth = p.second.first;
			bool ancestor_is_true = p.second.second | x->valid;
			if (depth == 16)
			{
				if (ancestor_is_true && x->left == NULL && x->right == NULL)
				{
					BCN16.push_back(0x8000 | (*(uint16_t*)x->hop.array));
					ancestor_is_true = false;
				}
				else
				{
					BCN16.push_back(++c16);
					ancestor_is_true = true;
				}
			}
			else if (depth == 24)
			{
				if (ancestor_is_true && x->left == NULL && x->right == NULL)
				{
					BCN24.push_back(0x8000 | (*(uint16_t*)x->hop.array));
					ancestor_is_true = false;
				}
				else
				{
					BCN24.push_back(++c24);
					ancestor_is_true = true;
				}
			}
			else if (depth == 32)
			{
				N32.push_back(*(uint16_t*)x->hop.array);
			}
			push(x, depth, ancestor_is_true);
			v.pop();
			if (depth < 32)
			{
				if (x->left != NULL)
				{
					if (ancestor_is_true == false)
					{
						int t = 1;
					}
					v.push(make_pair(x->left, make_pair(depth + 1, ancestor_is_true)));
				}
				if (x->right != NULL)
				{
					if (ancestor_is_true == false)
					{
						int t = 1;
					}
					v.push(make_pair(x->right, make_pair(depth + 1, ancestor_is_true)));
				}
			}
			delete x;
		}
	}
	vector<uint16_t> BCN16, BCN24, N32;
	TrieNode* trie_root;
};
