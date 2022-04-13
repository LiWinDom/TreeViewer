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

		this->recalcNodeHeight(node);
		this->recalcNodeHeight(t);
		node = t;
	}
	else if (balance < -1) {
		Node* t = node->right;
		node->right = t->left;
		t->left = node;

		this->recalcNodeHeight(node);
		this->recalcNodeHeight(t);
		node = t;
	}
	return;
}


AVLdrawer::AVLdrawer(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height) {
	this->windowX = x;
	this->windowY = y;
	this->width = width;
	this->height = height;

	this->border.setOutlineThickness(BORDER_SIZE);
	this->border.setFillColor(sf::Color(0));
	this->border.setOutlineColor(sf::Color(SELECTED_COLOR));
	this->border.setSize(sf::Vector2f(this->width - 2 * BORDER_SIZE, this->height - 2 * BORDER_SIZE));
	this->border.setPosition(this->windowX + BORDER_SIZE, this->windowY + BORDER_SIZE);

	this->left.setFillColor(sf::Color(BACKGROUND_COLOR));
	this->left.setSize(sf::Vector2f(this->windowX, WINDOW_HEIGHT));
	this->top.setFillColor(sf::Color(BACKGROUND_COLOR));
	this->top.setSize(sf::Vector2f(WINDOW_WIDTH, this->windowY));
	this->right.setFillColor(sf::Color(BACKGROUND_COLOR));
	this->right.setSize(sf::Vector2f(WINDOW_WIDTH - this->windowX - this->width, WINDOW_HEIGHT));
	this->right.setPosition(this->windowX + this->width, 0);
	this->bottom.setFillColor(sf::Color(BACKGROUND_COLOR));
	this->bottom.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - this->windowY - this->height));
	this->bottom.setPosition(0, this->windowY + this->height);

	font.loadFromFile("resourses/Consolas.ttf");

	return;
}

void AVLdrawer::eventProcessing(const sf::Event& event, const sf::Vector2i& mousePos) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Equal || event.key.code == sf::Keyboard::Add) {
			if (this->needScale < 6) ++this->needScale;
		}
		else if (event.key.code == sf::Keyboard::Hyphen || event.key.code == sf::Keyboard::Subtract) {
			if (this->needScale > 0) --this->needScale;
		}
		else if (event.key.code == sf::Keyboard::Up) {
			this->needY += 10 / scales[needScale];
		}
		else if (event.key.code == sf::Keyboard::Down) {
			this->needY -= 10 / scales[needScale];
		}
		else if (event.key.code == sf::Keyboard::Left) {
			this->needX += 10 / scales[needScale];
		}
		else if (event.key.code == sf::Keyboard::Right) {
			this->needX -= 10 / scales[needScale];
		}
	}

	return;
}

void AVLdrawer::draw(sf::RenderWindow& window) {
	this->curScale += (scales[needScale] - this->curScale) / 5.0;
	this->curX += (this->needX - this->curX) / 5.0;
	this->curY += (this->needY - this->curY) / 5.0;

	sf::CircleShape vertex;
	vertex.setOutlineThickness(BORDER_SIZE);
	vertex.setFillColor(sf::Color(0x00000000));
	vertex.setOutlineColor(sf::Color(SELECTED_COLOR));
	vertex.setRadius(this->convertSize(25));
	sf::FloatRect bounds = vertex.getLocalBounds();
	vertex.setOrigin(bounds.width / 2.0, bounds.height / 2.0);
	vertex.setPosition(this->convertCoordinate(this->curX, this->curY));
	window.draw(vertex);

	sf::Text text;
	text.setFont(this->font);
	text.setCharacterSize(convertSize(TEXT_SIZE / 2.0));
	text.setString(std::to_string(this->start->key));
	text.setFillColor(sf::Color(SELECTED_COLOR));
	bounds = text.getLocalBounds();
	text.setOrigin(bounds.width / 2.0, convertSize(TEXT_SIZE / 2.0) / 1.3);
	text.setPosition(convertCoordinate(this->curX, this->curY));
	window.draw(text);

	window.draw(this->border);
	window.draw(this->left);
	window.draw(this->top);
	window.draw(this->right);
	window.draw(this->bottom);

	return;
}

float AVLdrawer::convertSize(const float& size) {
	return size * this->curScale;
}

sf::Vector2f AVLdrawer::convertCoordinate(const float& coordX, const float& coordY) {
	return sf::Vector2f(coordX * this->curScale + this->windowX + this->width / 2.0, coordY * this->curScale + this->windowY + this->height / 2.0);
}