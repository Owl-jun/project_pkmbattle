#pragma once
#include <SFML/Graphics.hpp>

class BaseUI {
public:
    virtual ~BaseUI() = default;

    virtual void handleInput(const sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual bool isFocusable() const { return false; }
    virtual bool isFocused() const { return false; }
    virtual void setFocus(bool focus) {}

};
