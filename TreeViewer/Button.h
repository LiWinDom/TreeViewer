#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Config.h"

class Button {
public:
    Button(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height, const std::string& text);

    void draw(sf::RenderWindow& window);

    void select(const bool& selected = true);

    bool on(const uint16_t& curX, const uint16_t& curY);

private:
    uint16_t x, y;
    uint16_t width, height;

    std::string text = "";
    uint8_t selected = 0;

    sf::Font font;
    sf::RectangleShape border;
    sf::Text textText;
};