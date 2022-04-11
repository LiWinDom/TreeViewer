#pragma once
#include <algorithm>

class AVL
{
public:
	AVL();

	uint16_t getHeight();

	void insert(const uint16_t& key, const int& data);

	void inOrder();
	struct Node {
		uint16_t key, height = 1;
		int data;

		Node(const uint16_t& key, const int& data) {
			this->key = key;
			this->data = data;
			return;
		}

		Node* left = nullptr;
		Node* right = nullptr;
	};

	Node* start = nullptr;

private:

	uint16_t getNodeHeight(const Node* node);

	void recalcNodeHeight(Node*& node);

	void nodeInsert(Node*& node, const uint16_t& key, const int& data);
};
