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
	if (t1 == nullptr) return t1;
	if (t2 == nullptr) return t2;
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

uint16_t Treap::getNodeLayer(Node* node) {
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