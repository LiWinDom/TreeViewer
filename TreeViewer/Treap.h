#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Config.h"
#include "Error.h"

template <typename T1, typename T2, typename T3>
struct tuple {
    tuple(const T1& first, const T2& second, const T3& third) {
        this->first = first;
        this->second = second;
        this->third = third;
    }
    T1 first;
    T2 second;
    T3 third;
};

class Treap
{
public:
	struct Node {
		uint16_t key, priority = std::rand();
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

	void insert(const uint16_t& key, const int& data);

	void remove(const uint16_t& key);

protected:
    Node* merge(Node* t1, Node* t2);

	tuple<Node*, Node*, Node*> split(Node* t, const int& key);

	uint16_t getNodeHeight(const Node* node);

	uint16_t getNodeLayer(Node* node);
};


class TreapDrawer : public Treap {
public:
	TreapDrawer();

	void draw(sf::RenderWindow& window, const std::string& show = "key");

	std::pair<bool, uint16_t> eventProcessing(sf::RenderWindow& window, const sf::Event& event);

private:
	float curX = 0, curY = -WINDOW_HEIGHT / 4;
	float needX = 0, needY = -WINDOW_HEIGHT / 4;
	float curScale = 1;
	uint8_t needScale = 5;
	const float scales[7] = {0.05, 0.1, 0.2, 0.3, 0.5, 1, 2};

	sf::Vector2i mousePos;
	sf::Font font;

	float getNodeMagic(Node* node);

	float convertSize(const float& size);

	sf::Vector2f convertCoordinate(const float& coordX, const float& coordY);

	sf::Vector2f getNodeCoordinate(Node* node);

	void drawNode(sf::RenderWindow& window, Node* node, Node*& drawNodeInfo, const std::string& show = "key");
};
