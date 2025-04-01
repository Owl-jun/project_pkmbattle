#pragma once
#include "pch.h"

class BaseScene {
public:
    virtual ~BaseScene() = default;

    virtual void init() = 0;
    virtual void handleEvent(std::string tag, std::string msg) = 0;
    virtual void handleInput(const sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
