#include "AVL.h"

uint16_t AVL::getHeight() {
	if (start == nullptr) return 0;
	return start->height;
}

void AVL::insert(const uint16_t& key, const int& data) {
	this->insertNode(this->start, key, data);
	return;
}

void AVL::remove(const uint16_t& key) {
	this->removeNode(this->start, key);
	return;
}

uint16_t AVL::getNodeHeight(const Node* node) {
	if (node == nullptr) return 0;
	return node->height;
}

void AVL::recalcNodeHeight(Node*& node) {
	if (node == nullptr) return;
	this->recalcNodeHeight(node->left);
	this->recalcNodeHeight(node->right);
	uint16_t hLeft = this->getNodeHeight(node->left);
	uint16_t hRight = this->getNodeHeight(node->right);

	node->height = std::max(hLeft, hRight) + 1;
	return;
}

void AVL::rightRotate(Node*& node) {
	Node* t = node->left;
	node->left = t->right;
	t->right = node;
	node = t;
	this->recalcNodeHeight(t);
	return;
}

void AVL::leftRotate(Node*& node) {
	Node* t = node->right;
	node->right = t->left;
	t->left = node;
	node = t;
	this->recalcNodeHeight(t);
	return;
}

void AVL::insertNode(Node*& node, const uint16_t& key, const int& data) {
	if (node == nullptr) {
		node = new Node(key, data);
		return;
	}

	// Searching position
	if (key < node->key) {
		this->insertNode(node->left, key, data);
	}
	else if (key > node->key) {
		this->insertNode(node->right, key, data);
	}
	else {
		throw Error("AVL", "Equal keys are not allowed (inserting " + std::to_string(key) + ")");
	}
	this->recalcNodeHeight(node);

	// Balancing
	int8_t balance = this->getNodeHeight(node->left) - this->getNodeHeight(node->right);
	if (balance > 1 && key < node->left->key) {
		this->rightRotate(node);
	}
	else if (balance < -1 && key > node->right->key) {
		this->leftRotate(node);
	}
	// Big balancing
	else if (balance > 1 && key > node->left->key) {
		this->leftRotate(node->left);
		this->rightRotate(node);
	}
	else if (balance < -1 && key < node->right->key) {
		this->rightRotate(node->right);
		this->leftRotate(node);
	}
	return;
}

void AVL::removeNode(Node*& node, const uint16_t& key) {
	if (node == nullptr) {
		throw Error("AVL", "Node " + std::to_string(key) + " cannot be found");
	}

	// Searching node
	if (key < node->key) {
		this->removeNode(node->left, key);
	}
	else if (key > node->key) {
		this->removeNode(node->right, key);
	}
	else {
		// Deleting
		Node* q = node;
		if (node->left == nullptr && node->right == nullptr) {
			node = nullptr;
			delete q;
		}
		else if (node->left == nullptr) {
			node = node->right;
			delete q;
		}
		else if (node->right == nullptr) {
			node = node->left;
			delete q;
		}
		else {
			Node* p = node;
			Node* prev = node;
			if (this->getNodeHeight(node->left) > this->getNodeHeight(node->right)) {
				p = node->left;
				while (p->right != nullptr) {
					prev = p;
					p = p->right;
				}
				if (prev == node) node->left = p->left;
				else prev->right = p->left;
			}
			else {
				p = node->right;
				while (p->left != nullptr) {
					prev = p;
					p = p->left;
				}
				if (prev == node) node->right = p->right;
				else prev->left = p->right;
			}
			p->left = q->left;
			p->right = q->right;
			node = p;
			delete q;
		}
	}

	if (node != nullptr) {
		this->recalcNodeHeight(node);
		// Balancing
		int8_t balance = this->getNodeHeight(node->left) - this->getNodeHeight(node->right);
		if (balance > 1 && this->getNodeHeight(node->left->left) - this->getNodeHeight(node->left->right) >= 0) {
			this->rightRotate(node);
		}
		else if (balance < -1 && this->getNodeHeight(node->right->left) - this->getNodeHeight(node->right->right) <= 0) {
			this->leftRotate(node);
		}
		// Big balancing
		else if (balance > 1 && this->getNodeHeight(node->left->left) - this->getNodeHeight(node->left->right) < 0) {
			this->leftRotate(node->left);
			this->rightRotate(node);
		}
		else if (balance < -1 && this->getNodeHeight(node->right->left) - this->getNodeHeight(node->right->right) > 0) {
			this->rightRotate(node->right);
			this->leftRotate(node);
		}
	}
	return;
}

uint16_t AVL::getNodeLayer(Node* node) {
	if (node == nullptr) return 0;

	Node* q = this->start;
	uint16_t layer = 1;
	while (q->key != node->key) {
		if (node->key < q->key) q = q->left;
		else if (node->key > q->key) q = q->right;
		++layer;
	}
	return layer;
}


AVLDrawer::AVLDrawer() {
	font.loadFromFile("resourses/Consolas.ttf");
	return;
}

float AVLDrawer::getNodeMagic(Node* node) {
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

std::pair<bool, uint16_t> AVLDrawer::eventProcessing(sf::RenderWindow& window, const sf::Event& event) {
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

	static std::string mouseButton = "none";
	std::pair<bool, uint16_t> action(false, 0);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (mouseButton == "none" || mouseButton == "left" || mouseButton == "leftMove")) {
		if (mouseButton == "none") {
			mouseButton = "left";
		}
		else if (this->mousePos - sf::Mouse::getPosition(window) != sf::Vector2i(0, 0)) {
			this->needX -= (this->mousePos - sf::Mouse::getPosition(window)).x / this->curScale;
			this->needY -= (this->mousePos - sf::Mouse::getPosition(window)).y / this->curScale;
			mouseButton = "leftMove";
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mouseButton == "none") {
		mouseButton = "right";
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (mouseButton == "right") {
			Node* q = this->start;
			sf::Vector2f coordinate = this->getNodeCoordinate(q);
			while (coordinate.y - this->convertSize(VERTEX_SIZE) < this->mousePos.y && q != nullptr) {
				if (coordinate.x + convertSize(VERTEX_SIZE) < this->mousePos.x) {
					q = q->right;
					coordinate = this->getNodeCoordinate(q);
					continue;
				}
				else if (coordinate.x - convertSize(VERTEX_SIZE) > this->mousePos.x) {
					q = q->left;
					coordinate = this->getNodeCoordinate(q);
					continue;
				}
				else if (coordinate.x + convertSize(VERTEX_SIZE) > this->mousePos.x && coordinate.x - convertSize(VERTEX_SIZE) < this->mousePos.x &&
					coordinate.y + convertSize(VERTEX_SIZE) > this->mousePos.y && coordinate.y - convertSize(VERTEX_SIZE) < this->mousePos.y) {
					action = std::pair<bool, uint16_t>(true, q->key);
				}
				break;
			}
		}
		mouseButton = "none";
	}
	else if (event.type == sf::Event::MouseWheelMoved) {
		if (event.mouseWheel.delta > 0 && needScale < 6) {
			++needScale;
		}
		else if (event.mouseWheel.delta < 0 && needScale > 0) {
			--needScale;
		}
	}
	this->mousePos = sf::Mouse::getPosition(window);
	return action;
}

void AVLDrawer::draw(sf::RenderWindow& window, const std::string& show) {
	this->curScale += (scales[needScale] - this->curScale) / 5.0;
	this->curX += (this->needX - this->curX) / 5.0;
	this->curY += (this->needY - this->curY) / 5.0;

	Node* showInfoNode = nullptr;
	this->drawNode(window, this->start, showInfoNode, show);
	if (showInfoNode != nullptr) {
		sf::Text keyText, dataText;
		keyText.setFont(this->font);
		keyText.setCharacterSize(TEXT_SIZE);
		keyText.setFillColor(sf::Color(SELECTED_COLOR));
		dataText = keyText;
		keyText.setString("Key: " + std::to_string(showInfoNode->key));
		dataText.setString("Data: " + std::to_string(showInfoNode->data));
		sf::FloatRect keyTextBounds = keyText.getLocalBounds();
		sf::FloatRect dataTextBounds = dataText.getLocalBounds();
		keyText.setOrigin(-PADDING_SIZE, -PADDING_SIZE);
		keyText.setPosition(sf::Vector2f(mousePos));
		dataText.setOrigin(-PADDING_SIZE, -2 * PADDING_SIZE - keyTextBounds.height);
		dataText.setPosition(sf::Vector2f(mousePos));

		sf::RectangleShape rectangle;
		rectangle.setFillColor(sf::Color(BACKGROUND_COLOR & 0xFFFFFFA0));
		rectangle.setSize(sf::Vector2f(std::max(keyTextBounds.width, dataTextBounds.width) + 2 * PADDING_SIZE, keyTextBounds.height + dataTextBounds.height + 4 * PADDING_SIZE));
		rectangle.setOrigin(-PADDING_SIZE, -PADDING_SIZE);
		rectangle.setPosition(sf::Vector2f(mousePos));

		window.draw(rectangle);
		window.draw(keyText);
		window.draw(dataText);
	}
	return;
}

float AVLDrawer::convertSize(const float& size) {
	return size * this->curScale;
}

sf::Vector2f AVLDrawer::convertCoordinate(const float& coordX, const float& coordY) {
	return sf::Vector2f(convertSize(coordX + curX) + WINDOW_WIDTH / 2.0, convertSize(coordY + curY) + WINDOW_HEIGHT / 2.0);
}

sf::Vector2f AVLDrawer::getNodeCoordinate(Node* node) {
	if (node == nullptr) return sf::Vector2f(0, 0);
	return this->convertCoordinate(this->getNodeMagic(node) * std::pow(2, this->getHeight() - 1) * VERTEX_SIZE, this->getNodeLayer(node) * VERTEX_SIZE * 3);
}

void AVLDrawer::drawNode(sf::RenderWindow& window, Node* node, Node*& drawNodeInfo, const std::string& show) {
	if (node == nullptr) return;
	sf::Vector2f coordinate = this->getNodeCoordinate(node);

	if (coordinate.y > WINDOW_HEIGHT + convertSize(VERTEX_SIZE)) return;
	if (node->left != nullptr) {
		sf::VertexArray line(sf::LinesStrip, 2);
		line[0].position = coordinate;
		line[0].color = sf::Color(SELECTED_COLOR);
		line[1].position = this->getNodeCoordinate(node->left);
		line[1].color = sf::Color(SELECTED_COLOR);
		window.draw(line);
	}
	if (node->right != nullptr) {
		sf::VertexArray line(sf::LinesStrip, 2);
		line[0].position = coordinate;
		line[0].color = sf::Color(SELECTED_COLOR);
		line[1].position = this->getNodeCoordinate(node->right);
		line[1].color = sf::Color(SELECTED_COLOR);
		window.draw(line);
	}
	if (coordinate.x > -convertSize(VERTEX_SIZE) && coordinate.x < WINDOW_WIDTH + convertSize(VERTEX_SIZE) && coordinate.y > -convertSize(VERTEX_SIZE)) {
		sf::CircleShape circle;
		circle.setOutlineThickness(BORDER_SIZE);
		circle.setOutlineColor(sf::Color(SELECTED_COLOR));
		circle.setRadius(this->convertSize(VERTEX_SIZE));
		sf::FloatRect bounds = circle.getLocalBounds();
		circle.setOrigin(bounds.width / 2.0, bounds.height / 2.0);
		circle.setPosition(coordinate);

		sf::Text text;
		text.setFont(this->font);
		text.setCharacterSize(convertSize(TEXT_SIZE / 2.0));
		if (show == "key") text.setString(std::to_string(node->key));
		else if (show == "data") text.setString(std::to_string(node->data));
		text.setFillColor(sf::Color(SELECTED_COLOR));
		bounds = text.getLocalBounds();
		text.setOrigin(bounds.width / 2.0, bounds.height);
		text.setPosition(coordinate);

		if (this->mousePos.x > coordinate.x - this->convertSize(VERTEX_SIZE) && this->mousePos.x < coordinate.x + this->convertSize(VERTEX_SIZE) &&
			this->mousePos.y > coordinate.y - this->convertSize(VERTEX_SIZE) && this->mousePos.y < coordinate.y + this->convertSize(VERTEX_SIZE)) {
			drawNodeInfo = node;
			circle.setOutlineColor(sf::Color(HOVER_COLOR));
			text.setFillColor(sf::Color(HOVER_COLOR));
		}
		window.draw(circle);
		window.draw(text);
	}

	this->drawNode(window, node->left, drawNodeInfo, show);
	this->drawNode(window, node->right, drawNodeInfo, show);

	return;
}
