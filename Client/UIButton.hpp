#pragma once
#include "BaseUI.hpp"
#include <functional>

class UIButton : public BaseUI {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;
    sf::Font font;

public:

    UIButton(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& label, std::function<void()> clickFunc)
        : onClick(clickFunc) , font("C:/Windows/Fonts/arial.ttf") , text(font,label,24)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(sf::Color::Blue);

        font.openFromFile("C:/Windows/Fonts/arial.ttf"); 
        text.setFillColor(sf::Color::White);
        text.setPosition({ pos.x + 10, pos.y + 10 });
    }

    void handleEvent(const sf::Event& event) override {
        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse) {
                sf::Vector2f mousePos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
                if (shape.getGlobalBounds().contains(mousePos)) {
                    if (onClick)
                        onClick();
                }
            }
        }
    }

    void update() override {
        // hover ȿ�� ���� ���߿� �߰��� �� ����
    }

    void render(sf::RenderWindow& window) override {
        window.draw(shape);
        window.draw(text);
    }
};
