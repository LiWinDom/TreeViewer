#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "Config.h"
#include "AVL.h"
#include "Button.h"

std::string selected = "AVL";
sf::Font font;

AVLdrawer* AVLTree = new AVLdrawer();

Button* AVLButton = new Button(PADDING_SIZE, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 3 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "AVL");
Button* RBButton = new Button(4 * PADDING_SIZE + TEXT_SIZE * 3, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 2 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "RB");

Button* operationButton = new Button(WINDOW_WIDTH - TEXT_SIZE * 9 - 3 * PADDING_SIZE, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 9 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "Operation");

std::vector<Button*> buttons = {AVLButton, RBButton, operationButton};

bool load(const std::wstring& path) {
    std::ifstream file;

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
    font.loadFromFile("resourses/Consolas.ttf");
    //sf::Image icon;
    //icon.loadFromFile("resourses/icon.png");
    //window.setIcon(52, 52, icon.getPixelsPtr());
    window.setVerticalSyncEnabled(true);
    AVLButton->select();

    return;
}

void drawButtons(sf::RenderWindow& window) {
    for (uint8_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->draw(window);
    }
    return;
}

void display(sf::RenderWindow& window) {
    window.clear(sf::Color(BACKGROUND_COLOR));

    if (selected == "AVL") AVLTree->draw(window);

    drawButtons(window);
    window.display();

    return;
}

void clickEvent(sf::RenderWindow& window, uint16_t x, uint16_t y) {
    if (AVLButton->on(x, y)) {
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->select(false);
        }
        AVLButton->select();
        selected = "AVL";
    }
    else if (RBButton->on(x, y)) {
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->select(false);
        }
        RBButton->select();
        selected = "RB";
    }
    else if (operationButton->on(x, y)) {
        std::string line;
        std::getline(std::cin, line);

        int num1 = 0, num2 = 0;
        bool negativeData = false;
        std::string mode = "key";
        for (uint16_t i = 0; i < line.size(); ++i) {
            if (line[i] == ';') {
                if (mode == "data") {
                    if (negativeData) {
                        num2 = -num2;
                        negativeData = false;
                    }
                    try {
                        AVLTree->insert(num1, num2);
                        std::cout << "Added node " << num1 << ':' << num2 << std::endl;
                    }
                    catch (std::string err) {
                        std::cout << num1 << ":" << num2 << " - " << err << std::endl;
                    }
                    num1 = num2 = 0;
                    mode = "key";
                }
                else if (mode == "key") {
                    if (!AVLTree->remove(num1)) {
                        std::cout << "Removed node " << num1 << ':' << num2 << std::endl;
                    }
                    else {
                        std::cout << "Vertex " << num1 << " cannot be found" << std::endl;
                    }
                    num1 = 0;
                }
            }
            else if (mode == "key") {
                if (line[i] >= '0' && line[i] <= '9') {
                    num1 = num1 * 10 + line[i] - '0';
                }
                else if (line[i] == ':') {
                    mode = "data";
                }
                else {
                    std::cout << "Unexpected symbol at position " << i + 1 << " (" << line[i] << ")" << std::endl;
                    break;
                }
            }
            else if (mode == "data") {
                if (line[i] >= '0' && line[i] <= '9') {
                    num2 = num2 * 10 + line[i] - '0';
                }
                else if (line[i] == '-') {
                    negativeData = true;
                }
                else {
                    std::cout << "Unexpected symbol at position " << i + 1 << " (" << line[i] << ")" << std::endl;
                    break;
                }
            }
        }
    }
    return;
}

void eventProcessing(sf::RenderWindow& window) {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (mousePos.x < 0 || mousePos.y < 0 || mousePos.x > window.getSize().x || mousePos.y > window.getSize().y) continue;
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->on(mousePos.x, mousePos.y);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            clickEvent(window, mousePos.x, mousePos.y);
        }

        if (selected == "AVL") AVLTree->eventProcessing(event, mousePos);
    }
    return;
}

int main() {
    // Test tree
    /*for (uint16_t i = 0; i < 20; ++i) {
        uint16_t key = std::rand();
        int data = std::rand();
        try {
            AVLTree->insert(key, data);
            std::cout << i + 1 << " - Added node " << key << ':' << data << std::endl;
        }
        catch (std::string err) {
            std::cout << i + 1 << " - Node with key " << key << " is already exists" << std::endl;
        }
    }*/

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tree Viewer [0.3]", sf::Style::Close);
    onStart(window);

    while (window.isOpen()) {
        eventProcessing(window);
        display(window);
    }
    return 0;
}