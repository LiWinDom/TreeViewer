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
	if (balance > 1 && key < node->left->key) {
		Node* t = node->left;
		node->left = t->right;
		t->right = node;
		node = t;

		this->recalcNodeHeight(t->right);
		this->recalcNodeHeight(t);
	}
	else if (balance < -1 && key > node->right->key) {
		Node* t = node->right;
		node->right = t->left;
		t->left = node;
		node = t;

		this->recalcNodeHeight(t->left);
		this->recalcNodeHeight(t);
	}
	// Big balancing
	else if (balance > 1 && key > node->left->key) {
		Node* q = node;
		Node* p = node->left;
		node = p->right;
		node->left = p;
		node->right = q;
		q->left = nullptr;
		p->right = nullptr;

		this->recalcNodeHeight(q);
		this->recalcNodeHeight(p);
		this->recalcNodeHeight(node);
	}
	else if (balance < -1 && key < node->right->key) {
		Node* q = node;
		Node* p = node->right;
		node = p->left;
		node->left = q;
		node->right = p;
		q->right = nullptr;
		p->left = nullptr;

		this->recalcNodeHeight(q);
		this->recalcNodeHeight(p);
		this->recalcNodeHeight(node);
	}
	return;
}

uint16_t AVL::getNodeLayer(Node* node) {
	if (node == nullptr) return 0;

	Node* q = this->start;
	uint16_t layer = 0;
	while (q->key != node->key) {
		if (node->key < q->key) q = q->left;
		else if (node->key > q->key) q = q->right;
		++layer;
	}
	return layer;
}

float AVL::getNodeMagic(Node* node) {
	if (node == nullptr) return 0;

	float mult = 1;
	float total = 0;

	Node* q = this->start;
	while (q->key != node->key) {
		if (node->key < q->key) {
			total -= mult;
			q = q->left;
		}
		else if (node->key > q->key) {
			total += mult;
			q = q->right;
		}
		mult /= 2;
	}
	return total;
}


AVLdrawer::AVLdrawer(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height) {
	this->windowX = x;
	this->windowY = y;
	this->width = width;
	this->height = height;

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

void AVLdrawer::draw(sf::RenderWindow& window, const std::string& show) {
	this->curScale += (scales[needScale] - this->curScale) / 5.0;
	this->curX += (this->needX - this->curX) / 5.0;
	this->curY += (this->needY - this->curY) / 5.0;

	this->drawNode(window, this->start, show);
	return;
}

float AVLdrawer::convertSize(const float& size) {
	return size * this->curScale;
}

sf::Vector2f AVLdrawer::convertCoordinate(const float& coordX, const float& coordY) {
	return sf::Vector2f(convertSize(coordX + curX) + this->windowX + this->width / 2.0, convertSize(coordY + curY) + this->windowY + this->height / 2.0);
}

sf::Vector2f AVLdrawer::getNodeCoordinate(Node* node) {
	if (node == nullptr) return sf::Vector2f(0, 0);
	return this->convertCoordinate(this->getNodeMagic(node) * std::pow(2, this->getHeight() - 1) * VERTEX_SIZE, this->getNodeLayer(node) * VERTEX_SIZE * 3);
}

void AVLdrawer::drawNode(sf::RenderWindow& window, Node* node, const std::string& show) {
	if (node == nullptr) return;
	sf::Vector2f coordinate = this->getNodeCoordinate(node);

	if (coordinate.y > WINDOW_HEIGHT + convertSize(VERTEX_SIZE)) return;
	if (coordinate.x > -convertSize(VERTEX_SIZE) && coordinate.x < WINDOW_WIDTH + convertSize(VERTEX_SIZE) && coordinate.y > -convertSize(VERTEX_SIZE)) {
		sf::CircleShape vertex;
		vertex.setOutlineThickness(BORDER_SIZE);
		vertex.setFillColor(sf::Color(BACKGROUND_COLOR));
		vertex.setOutlineColor(sf::Color(SELECTED_COLOR));
		vertex.setRadius(this->convertSize(VERTEX_SIZE));
		sf::FloatRect bounds = vertex.getLocalBounds();
		vertex.setOrigin(bounds.width / 2.0, bounds.height / 2.0);
		vertex.setPosition(coordinate);
		window.draw(vertex);

		sf::Text text;
		text.setFont(this->font);
		text.setCharacterSize(convertSize(TEXT_SIZE / 2.0));
		if (show == "key") text.setString(std::to_string(node->key));
		else if (show == "data") text.setString(std::to_string(node->data));
		text.setFillColor(sf::Color(SELECTED_COLOR));
		bounds = text.getLocalBounds();
		text.setOrigin(bounds.width / 2.0, bounds.height);
		text.setPosition(coordinate);
		window.draw(text);
	}

	this->drawNode(window, node->left, show);
	this->drawNode(window, node->right, show);

	return;
}