#pragma once
#include <SFML/Graphics.hpp>

class BaseUI {
public:
    virtual ~BaseUI() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
