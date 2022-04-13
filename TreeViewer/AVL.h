#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Config.h"

class AVL
{
public:
	uint16_t getHeight();

	void insert(const uint16_t& key, const int& data);

protected:
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

	uint16_t getNodeLayer(Node* node);

	float getNodeMagic(Node* node);
};


class AVLdrawer : public AVL {
public:
	AVLdrawer(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height);

	void draw(sf::RenderWindow& window, const std::string& show = "key");

	void eventProcessing(const sf::Event& event, const sf::Vector2i& mousePos);

private:
	uint16_t windowX, windowY;
	uint16_t width, height;

	float needX = 0, needY = 0;
	float curX = 0, curY = 0;
	uint8_t needScale = 4;
	float curScale = 1;
	const float scales[7] = { 0.01, 0.1, 0.3, 0.5, 1, 2, 3 };

	sf::Font font;

	float convertSize(const float& size);

	sf::Vector2f convertCoordinate(const float& coordX, const float& coordY);

	sf::Vector2f getNodeCoordinate(Node* node);

	void drawNode(sf::RenderWindow& window, Node* node, const std::string& show = "key");
};
