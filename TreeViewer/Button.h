#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Config.h"

class Button {
private:
    std::string text = "";
    bool selected = false;
    sf::Font font;
public:
    uint16_t x, y;
    uint16_t width, height;

    Button(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height, const std::string& text);

    void draw(sf::RenderWindow& window);

    bool on(const uint16_t& curX, const uint16_t& curY);
};