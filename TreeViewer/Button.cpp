#include "Button.h"

Button::Button(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height, const std::string& text) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->text = text;
        font.loadFromFile("resourses/Consolas.ttf");
    }

void Button::draw(sf::RenderWindow& window) {
        sf::RectangleShape border;
        sf::Text textText;

        border.setOutlineThickness(BORDER_SIZE);
        border.setOutlineColor(sf::Color(selected ? PADDING_SIZE : TEXT_COLOR));
        border.setSize(sf::Vector2f(width - 2 * BORDER_SIZE, height - 2 * BORDER_SIZE));
        border.setPosition(x + BORDER_SIZE, y + BORDER_SIZE);
        border.setFillColor(sf::Color(BACKGROUND_COLOR));

        textText.setFont(font);
        textText.setCharacterSize(TEXT_SIZE);
        textText.setFillColor(sf::Color(selected ? PADDING_SIZE : TEXT_COLOR));
        textText.setString(text);
        sf::FloatRect bounds = textText.getLocalBounds();
        textText.setOrigin(bounds.width / 2, TEXT_SIZE / 1.75);
        textText.setPosition(x + width / 2 - BORDER_SIZE, y + height / 2 - BORDER_SIZE);

        window.draw(border);
        window.draw(textText);

        return;
    }

bool Button::on(const uint16_t& curX, const uint16_t& curY) {
    if (curX >= x && curX <= x + width && curY >= y && curY <= y + height) {
        return selected = true;
    }
    return selected = false; 
}