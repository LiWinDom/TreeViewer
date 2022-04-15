#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Config.h"

class AVL
{
public:
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

	uint16_t getHeight();

	void insert(const uint16_t& key, const int& data);

	bool remove(const uint16_t& key);

protected:
	uint16_t getNodeHeight(const Node* node);

	void recalcNodeHeight(Node*& node);

	void rightRotate(Node*& node);

	void leftRotate(Node*& node);

	void insertNode(Node*& node, const uint16_t& key, const int& data);

	bool removeNode(Node*& node, const uint16_t& key);

	uint16_t getNodeLayer(Node* node);
};


class AVLdrawer : public AVL {
public:
	AVLdrawer();

	void draw(sf::RenderWindow& window, const std::string& show = "key");

	void eventProcessing(sf::RenderWindow& window, const sf::Event& event);

private:
	float curX = 0, curY = 0;
	float needX = 0, needY = 0;
	float curScale = 1;
	uint8_t needScale = 5;
	const float scales[7] = { 0.05, 0.1, 0.2, 0.3, 0.5, 1, 2};

	sf::Vector2i mousePos;
	sf::Font font;

	float getNodeMagic(Node* node);

	float convertSize(const float& size);

	sf::Vector2f convertCoordinate(const float& coordX, const float& coordY);

	sf::Vector2f getNodeCoordinate(Node* node);

	void drawNode(sf::RenderWindow& window, Node* node, Node*& drawNodeInfo, const std::string& show = "key");
};
