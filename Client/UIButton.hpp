#pragma once
#include "BaseUI.hpp"
#include <functional>

class UIButton : public BaseUI {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;
    sf::Color defaultColor;

public:

    UIButton(const sf::Vector2f& pos, 
            const sf::Vector2f& size, 
            const std::string& label, 
            sf::Color Color, 
            sf::Font& sharedFont,
            std::function<void()> clickFunc)
        : onClick(clickFunc) 
        , text(sharedFont,label,48)
        , defaultColor(Color)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Color);

        text.setFillColor(sf::Color::Black);
        const auto bounds = text.getLocalBounds();  
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
            shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse && 
                mouse->button == sf::Mouse::Button::Left && 
                shape.getGlobalBounds().contains(mousePos)) {
                    if (onClick) onClick();
            }
        }
        if (event.is<sf::Event::KeyPressed>()) {
            if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Enter)) {
                if (onClick) onClick();
            }
        }
    }

    void update(sf::RenderWindow& window) override {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);  
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(defaultColor); 
        }
    }

    void render(sf::RenderWindow& window) override {
        window.draw(shape);
        window.draw(text);
    }
};
