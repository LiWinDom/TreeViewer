#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "Config.h"
#include "Error.h"
#include "AVL.h"
#include "Treap.h"
#include "Button.h"

std::string selected = "AVL";
sf::Font font;

AVLDrawer* AVLTree = new AVLDrawer();
TreapDrawer* TreapTree = new TreapDrawer();

Button* AVLButton = new Button(PADDING_SIZE, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 3 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "AVL");
Button* RBButton = new Button(4 * PADDING_SIZE + TEXT_SIZE * 3, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 2 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "RB");
Button* treapButton = new Button(7 * PADDING_SIZE + TEXT_SIZE * 5, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 5 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "Treap");
Button* splayButton = new Button(10 * PADDING_SIZE + TEXT_SIZE * 10, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 5 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "Splay");

Button* operationButton = new Button(WINDOW_WIDTH - TEXT_SIZE * 9 - 3 * PADDING_SIZE, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 9 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "Operation");

std::vector<Button*> buttons = {AVLButton, RBButton, treapButton, splayButton, operationButton};

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
    std::srand(std::time(nullptr));

    font.loadFromFile("resourses/Consolas.ttf");
    //sf::Image icon;
    //icon.loadFromFile("resourses/icon.png");
    //window.setIcon(52, 52, icon.getPixelsPtr());
    window.setVerticalSyncEnabled(true);
    AVLButton->select();

    return;
}

void display(sf::RenderWindow& window) {
    window.clear(sf::Color(BACKGROUND_COLOR));

    if (selected == "AVL") AVLTree->draw(window);
    else if (selected == "treap") TreapTree->draw(window);

    for (uint8_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->draw(window);
    }

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
    else if (treapButton->on(x, y)) {
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->select(false);
        }
        treapButton->select();
        selected = "treap";
    }
    else if (splayButton->on(x, y)) {
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->select(false);
        }
        splayButton->select();
        selected = "splay";
    }
    else if (operationButton->on(x, y)) {
        std::string line;
        std::getline(std::cin, line);
        if (line[line.size() - 1] != ';') line.push_back(';');

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
                        TreapTree->insert(num1, num2);
                        std::cout << "Added node " << num1 << ':' << num2 << std::endl;
                    }
                    catch (Error* err) {
                        std::cout << err->source << ": " << err->message << std::endl;
                        delete err;
                    }
                }
                else if (mode == "key") {
                    try {
                        AVLTree->remove(num1);
                        TreapTree->remove(num1);
                        std::cout << "Removed node " << num1 << std::endl;
                    }
                    catch (Error* err) {
                        std::cout << err->source << ": " << err->message << std::endl;
                        delete err;
                    }
                }
                else if (mode == "generate") {
                    for (uint16_t i = 0; i < num1; ++i) {
                        uint16_t key = std::rand();
                        int data = std::rand();
                        try {
                            AVLTree->insert(key, data);
                            TreapTree->insert(key, data);
                            std::cout << "Generated node " << key << ":" << data << std::endl;
                        }
                        catch (Error* err) {
                            std::cout << err->source << ": " << err->message << std::endl;
                            delete err;
                        }
                    }
                }
                else if (mode == "delete all") {
                    while (AVLTree->start != nullptr) {
                        AVLTree->remove(AVLTree->start->key);
                    }
                    while (TreapTree->start != nullptr) {
                        TreapTree->remove(TreapTree->start->key);
                    }
                    std::cout << "Deleted all nodes" << std::endl;
                }
                num1 = num2 = 0;
                mode = "key";
            }
            else if (line[i] == 'g') {
                mode = "generate";
            }
            else if (line[i] == 'x') {
                mode = "delete all";
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
            else if (mode == "generate") {
                if (line[i] >= '0' && line[i] <= '9') {
                    num1 = num1 * 10 + line[i] - '0';
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
    static std::string mouseButton = "none";
    static sf::Vector2i lastMousePos;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->on(mousePos.x, mousePos.y);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (mouseButton == "none" || mouseButton == "left" || mouseButton == "leftMove")) {
            if (mouseButton == "none") {
                mouseButton = "left";
            }
            else if (lastMousePos - sf::Mouse::getPosition() != sf::Vector2i(0, 0)) {
                mouseButton = "leftMove";
            }
            lastMousePos = sf::Mouse::getPosition();
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mouseButton == "none") {
            mouseButton = "right";
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (mouseButton == "left") {
                clickEvent(window, mousePos.x, mousePos.y);
            }
            mouseButton = "none";
        }

        std::pair<bool, uint16_t> action;
        if (selected == "AVL") action = AVLTree->eventProcessing(window, event);
        if (selected == "treap") action = TreapTree->eventProcessing(window, event);

        if (action.first) {
            AVLTree->remove(action.second);
            TreapTree->remove(action.second);
        }
    }
    return;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tree Viewer [0.3]", sf::Style::Close);
    onStart(window);

    while (window.isOpen()) {
        eventProcessing(window);
        display(window);
    }
    return 0;
}