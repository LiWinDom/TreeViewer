#include "Button.h"

Button::Button(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height, const std::string& text) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->text = text;
    font.loadFromFile("resourses/Consolas.ttf");
    return;
}

void Button::draw(sf::RenderWindow& window) {
    this->border.setOutlineThickness(BORDER_SIZE);
    this->border.setFillColor(sf::Color(0));
    this->border.setSize(sf::Vector2f(width - 2 * BORDER_SIZE, height - 2 * BORDER_SIZE));
    this->border.setPosition(x + BORDER_SIZE, y + BORDER_SIZE);

    this->textText.setFont(font);
    this->textText.setCharacterSize(TEXT_SIZE);
    this->textText.setString(text);
    sf::FloatRect bounds = textText.getLocalBounds();
    this->textText.setOrigin(bounds.width / 2, TEXT_SIZE / 1.75);
    this->textText.setPosition(x + width / 2 - BORDER_SIZE, y + height / 2 - BORDER_SIZE);

    switch (selected) {
    case 0:
        this->border.setOutlineColor(sf::Color(INACTIVE_COLOR));
        this->textText.setFillColor(sf::Color(INACTIVE_COLOR));
        break;
    case 1:
        this->border.setOutlineColor(sf::Color(HOVER_COLOR));
        this->textText.setFillColor(sf::Color(HOVER_COLOR));
        break;
    case 2:
        this->border.setOutlineColor(sf::Color(SELECTED_COLOR));
        this->textText.setFillColor(sf::Color(SELECTED_COLOR));
        break;
    }

    window.draw(border);
    window.draw(textText);

    return;
}

void Button::select(const bool& selected) {
    if (selected) {
        this->selected = 2;
        return;
    }
    this->selected = 0;
    return;
}

bool Button::on(const uint16_t& curX, const uint16_t& curY) {
    if (curX >= x && curX <= x + width && curY >= y && curY <= y + height) {
        if (this->selected != 2) this->selected = 1;
        return true;
    }
    if (this->selected != 2) this->selected = 0;
    return false;
}