#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "Config.h"
#include "AVL.h"
#include "Button.h"

sf::Font font;

bool load(const std::wstring& path) {
    std::ifstream file;
    std::vector<std::wstring> loaded(256, L"");

    file.open(path);
    if (!file.is_open()) {
        std::ofstream temp;
        temp.open(path);
        if (!temp.is_open()) {
            return false;
        }
        temp.close();
    }

    file.close();

    return true;
}

bool save(const std::wstring& path) {
    std::ofstream file;
    file.open(path);
    if (!file.is_open()) {
        return false;
    }

    file.close();

    return true;
}

void onStart(sf::RenderWindow& window) {
    /*font.loadFromFile("resourses/minecraft.ttf");
    sf::Image icon;
    icon.loadFromFile("resourses/icon.png");
    window.setIcon(52, 52, icon.getPixelsPtr());
    window.setVerticalSyncEnabled(true);

    for (uint8_t i = 0;; ++i) {
        lines[i] = new std::wstring();
        if (i == 255) break;
    }
    return;*/
}

void drawButtons(sf::RenderWindow& window) {
    /*for (uint8_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->draw(window);
    }*/
    return;
}

void display(sf::RenderWindow& window) {
    window.clear(sf::Color(BACKGROUND_COLOR));

    drawButtons(window);
    window.display();

    return;
}

void clickEvent(sf::RenderWindow& window, uint16_t x, uint16_t y) {
}

void eventProcessing(sf::RenderWindow& window) {
    sf::Event event;
    static bool isMousePressed = false;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        else if (event.type == sf::Event::KeyPressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (mousePos.x < 0 || mousePos.y < 0 || mousePos.x > window.getSize().x || mousePos.y > window.getSize().y) continue;
        /*for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->on(mousePos.x, mousePos.y);
        }*/

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMousePressed) {
            clickEvent(window, mousePos.x, mousePos.y);
            isMousePressed = true;
        }
        if (event.type == sf::Event::MouseButtonReleased) isMousePressed = false;
    }

    return;
}

void inOrder(AVL::Node* q) {
    if (q == nullptr) return;
    inOrder(q->left);
    std::cout << q->key << "  | " << q->data << std::endl;
    inOrder(q->right);
    return;
}

int main() {
    AVL AVLtree;

    AVLtree.insert(10, 1);
    AVLtree.insert(20, 2);
    AVLtree.insert(30, 3);
    AVLtree.insert(40, 4);
    AVLtree.insert(50, 5);
    AVLtree.insert(25, 2);
    AVLtree.insert(23, 3);
    AVLtree.insert(12, 4);
    AVLtree.insert(54, 7);
    AVLtree.insert(44, 2);

    std::cout << " AVL TREE" << std::endl;
    std::cout << "(in-order)" << std::endl;
    std::cout << "__________" << std::endl;
    std::cout << "Key | Data" << std::endl;
    std::cout << "----+-----" << std::endl;
    AVL::Node* q = AVLtree.start;
    inOrder(q);


    /*sf::RenderWindow window(sf::VideoMode(editorWidth + 4 * PADDING_SIZE + editorWidth / 2 + 2 * BORDER_SIZE, editorHeight + 2 * (PADDING_SIZE + BORDER_SIZE)), "Tree Viewer [0.1]", sf::Style::Close);

    onStart(window);

    while (window.isOpen()) {
        eventProcessing(window);
        display(window);
    }*/


    return 0;
}