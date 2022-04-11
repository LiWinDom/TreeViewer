#include "AVL.h"

uint16_t AVL::getHeight() {
	if (start == nullptr) return 0;
	return start->height;
}

void AVL::insert(const uint16_t& key, const int& data) {
	this->nodeInsert(this->start, key, data);
	return;
}

uint16_t AVL::getNodeHeight(const Node* node) {
	if (node == nullptr) return 0;
	return node->height;
}

void AVL::recalcNodeHeight(Node*& node) {
	uint16_t hLeft = this->getNodeHeight(node->left);
	uint16_t hRight = this->getNodeHeight(node->right);

	node->height = std::max(hLeft, hRight) + 1;
	return;
}

void AVL::nodeInsert(Node*& node, const uint16_t& key, const int& data) {
	if (node == nullptr) {
		node = new Node(key, data);
		return;
	}

	// Searching position
	if (key < node->key) {
		this->nodeInsert(node->left, key, data);
	}
	else if (key > node->key) {
		this->nodeInsert(node->right, key, data);
	}
	else {
		throw "Equal keys are not allowed";
		return;
	}
	this->recalcNodeHeight(node);

	// Balancing
	int8_t balance = this->getNodeHeight(node->left) - this->getNodeHeight(node->right);
	if (balance > 1) {
		Node* t = node->left;
		node->left = t->right;
		t->right = node;
		node = t;

		this->recalcNodeHeight(node);
		this->recalcNodeHeight(t);
	}
	else if (balance < -1) {
		Node* t = node->right;
		node->right = t->left;
		t->left = node;
		node = t;

		this->recalcNodeHeight(node);
		this->recalcNodeHeight(t);
	}
	return;
}


AVLdrawer::AVLdrawer(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->border.setOutlineThickness(BORDER_SIZE);
	this->border.setFillColor(sf::Color(BACKGROUND_COLOR));
	this->border.setOutlineColor(sf::Color(SELECTED_COLOR));
	this->border.setSize(sf::Vector2f(width - 2 * BORDER_SIZE, height - 2 * BORDER_SIZE));
	this->border.setPosition(x + BORDER_SIZE, y + BORDER_SIZE);
	return;
}

void AVLdrawer::draw(sf::RenderWindow& window) {
	window.draw(this->border);
	return;
}