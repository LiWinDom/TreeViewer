#include "Treap.h"

void Treap::insert(const uint16_t& key, const int& data) {
	tuple<Node*, Node*, Node*> splt = this->split(this->start, key);
	if (splt.second != nullptr) {
		this->start = this->merge(splt.first, this->merge(splt.second, splt.third));
		throw new Error("Treap", "Equal keys are not allowed (inserting " + std::to_string(key) + ")");
	}
	this->start = this->merge(splt.first, this->merge(new Node(key, data), splt.third));
	return;
}

void Treap::remove(const uint16_t& key) {
	tuple<Node*, Node*, Node*> splt = this->split(this->start, key);
	if (splt.second == nullptr) {
		throw new Error("Treap", "Node " + std::to_string(key) + " cannot be found");
	}
	delete splt.second;
	this->start = this->merge(splt.first, splt.third);
	return;
}

Treap::Node* Treap::merge(Node* t1, Node* t2) {
	if (t1 == nullptr) return t2;
	if (t2 == nullptr) return t1;
	if (t1->priority > t2->priority) {
		t1->right = merge(t1->right, t2);
		return t1;
	}
	t2->left = merge(t1, t2->left);
	return t2;
}

tuple<Treap::Node*, Treap::Node*, Treap::Node*> Treap::split(Node* t, const int& key) {
	if (t == nullptr) return tuple<Node*, Node*, Node*>(nullptr, nullptr, nullptr);
	if (t->key < key) {
		tuple<Node*, Node*, Node*> splt = split(t->right, key);
		t->right = nullptr;
		splt.first = merge(t, splt.first);
		return splt;
	}
	if (t->key > key) {
		tuple<Node*, Node*, Node*> splt = split(t->left, key);
		t->left = nullptr;
		splt.third = merge(splt.third, t);
		return splt;
	}	
	Node* left = t->left;
	Node* right = t->right;
	t->left = nullptr;
	t->right = nullptr;
	return tuple<Node*, Node*, Node*>(left, t, right);
}

uint16_t Treap::getNodeHeight(const Node* node) {
	if (node == nullptr) return 0;
	return std::max(this->getNodeHeight(node->left), this->getNodeHeight(node->right)) + 1;
}

uint16_t Treap::getNodeLayer(Node* node) {
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


TreapDrawer::TreapDrawer() {
	font.loadFromFile("resourses/Consolas.ttf");
	return;
}

float TreapDrawer::getNodeMagic(Node* node) {
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

std::pair<bool, uint16_t> TreapDrawer::eventProcessing(sf::RenderWindow& window, const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Equal || event.key.code == sf::Keyboard::Add) {
			if (this->needScale < 6)++this->needScale;
		}
		else if (event.key.code == sf::Keyboard::Hyphen || event.key.code == sf::Keyboard::Subtract) {
			if (this->needScale > 0)--this->needScale;
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

void TreapDrawer::draw(sf::RenderWindow& window, const std::string& show) {
	this->curScale += (scales[needScale] - this->curScale) / 5.0;
	this->curX += (this->needX - this->curX) / 5.0;
	this->curY += (this->needY - this->curY) / 5.0;

	Node* showInfoNode = nullptr;
	this->drawNode(window, this->start, showInfoNode, show);
	if (showInfoNode != nullptr) {
		sf::Text keyText, prioText, dataText;
		keyText.setFont(this->font);
		keyText.setCharacterSize(TEXT_SIZE);
		keyText.setFillColor(sf::Color(SELECTED_COLOR));
		dataText = prioText = keyText;
		keyText.setString("Key: " + std::to_string(showInfoNode->key));
		prioText.setString("Priority: " + std::to_string(showInfoNode->priority));
		dataText.setString("Data: " + std::to_string(showInfoNode->data));
		sf::FloatRect keyTextBounds = keyText.getLocalBounds();
		sf::FloatRect prioTextBounds = prioText.getLocalBounds();
		sf::FloatRect dataTextBounds = dataText.getLocalBounds();
		keyText.setOrigin(-PADDING_SIZE, -PADDING_SIZE);
		keyText.setPosition(sf::Vector2f(mousePos));
		prioText.setOrigin(-PADDING_SIZE, -2 * PADDING_SIZE - keyTextBounds.height);
		prioText.setPosition(sf::Vector2f(mousePos));
		dataText.setOrigin(-PADDING_SIZE, -2 * PADDING_SIZE - keyTextBounds.height - prioTextBounds.height);
		dataText.setPosition(sf::Vector2f(mousePos));

		sf::RectangleShape rectangle;
		rectangle.setFillColor(sf::Color(BACKGROUND_COLOR & 0xFFFFFFA0));
		rectangle.setSize(sf::Vector2f(std::max(keyTextBounds.width, std::max(prioTextBounds.width, dataTextBounds.width)) + 2 * PADDING_SIZE, keyTextBounds.height + prioTextBounds.height + dataTextBounds.height + 4 * PADDING_SIZE));
		rectangle.setOrigin(-PADDING_SIZE, -PADDING_SIZE);
		rectangle.setPosition(sf::Vector2f(mousePos));

		window.draw(rectangle);
		window.draw(keyText);
		window.draw(prioText);
		window.draw(dataText);
	}
	return;
}

float TreapDrawer::convertSize(const float& size) {
	return size * this->curScale;
}

sf::Vector2f TreapDrawer::convertCoordinate(const float& coordX, const float& coordY) {
	return sf::Vector2f(convertSize(coordX + curX) + WINDOW_WIDTH / 2.0, convertSize(coordY + curY) + WINDOW_HEIGHT / 2.0);
}

sf::Vector2f TreapDrawer::getNodeCoordinate(Node* node) {
	if (node == nullptr) return sf::Vector2f(0, 0);
	return this->convertCoordinate(this->getNodeMagic(node) * std::pow(2, this->getNodeHeight(this->start) - 1) * VERTEX_SIZE, this->getNodeLayer(node) * VERTEX_SIZE * 3);
}

void TreapDrawer::drawNode(sf::RenderWindow& window, Node* node, Node*& drawNodeInfo, const std::string& show) {
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