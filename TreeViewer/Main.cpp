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

AVLdrawer* AVLtree = new AVLdrawer(PADDING_SIZE, PADDING_SIZE, WINDOW_WIDTH - 2 * PADDING_SIZE, WINDOW_HEIGHT - 5 * PADDING_SIZE - TEXT_SIZE);

Button* AVLbutton = new Button(PADDING_SIZE, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 3 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "AVL");
Button* RBbutton = new Button(4 * PADDING_SIZE + TEXT_SIZE * 3, WINDOW_HEIGHT - 3 * PADDING_SIZE - TEXT_SIZE, TEXT_SIZE * 2 + 2 * PADDING_SIZE, TEXT_SIZE + 2 * PADDING_SIZE, "RB");

std::vector<Button*> buttons = {AVLbutton, RBbutton};

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

    if (selected == "AVL") AVLtree->draw(window);

    drawButtons(window);
    window.display();

    return;
}

void clickEvent(sf::RenderWindow& window, uint16_t x, uint16_t y) {
    if (AVLbutton->on(x, y)) {
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->select(false);
        }
        AVLbutton->select();
        selected = "AVL";
    }
    else if (RBbutton->on(x, y)) {
        for (uint8_t i = 0; i < buttons.size(); ++i) {
            buttons[i]->select(false);
        }
        RBbutton->select();
        selected = "RB";
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

        if (selected == "AVL") AVLtree->eventProcessing(event, mousePos);
    }
    return;
}

int main() {
    for (uint8_t i = 0; i < 10; ++i) {
        AVLtree->insert(i, std::rand());
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tree Viewer [0.1]", sf::Style::Close);
    AVLbutton->select();
    onStart(window);

    while (window.isOpen()) {
        eventProcessing(window);
        display(window);
    }


    return 0;
}