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
};


class AVLdrawer : public AVL {
public:
	AVLdrawer(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height);

	void draw(sf::RenderWindow& window);

	void eventProcessing(const sf::Event& event, const sf::Vector2i& mousePos);

private:
	uint16_t windowX, windowY;
	uint16_t width, height;

	float needX = 0, needY = 0;
	float curX = 0, curY = 0;
	uint8_t needScale = 4;
	float curScale = 1;

	const float scales[7] = { 0.05, 0.1, 0.3, 0.5, 1, 2, 3 };
	sf::Font font;

	sf::RectangleShape border;
	sf::RectangleShape left, top, right, bottom;

	float convertSize(const float& size);

	sf::Vector2f convertCoordinate(const float& coordX, const float& coordY);

	void drawSubtree(sf::RenderWindow& window, const Node& subtree);
};
