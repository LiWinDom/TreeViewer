#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Config.h"

class AVL
{
public:
	uint16_t getHeight();

	void insert(const uint16_t& key, const int& data);

private:
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

	uint16_t getNodeHeight(const Node* node);

	void recalcNodeHeight(Node*& node);

	void nodeInsert(Node*& node, const uint16_t& key, const int& data);
};


class AVLdrawer : public AVL {
public:
	AVLdrawer(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height);

	void draw(sf::RenderWindow& window);

private:
	uint16_t x, y;
	uint16_t width, height;

	sf::RectangleShape border;
};
