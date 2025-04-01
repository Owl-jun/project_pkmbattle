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
            std::function<void()> clickFunc,
            unsigned int textSize = 48)
        : onClick(clickFunc) 
        , text(sharedFont,label,textSize)
        , defaultColor(Color)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Color);

        text.setFillColor(sf::Color::Black);
        const auto bounds = text.getLocalBounds();  // 텍스트 크기를 가져옴
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });   // 텍스트 중심을 기준으로 위치를 설정
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
            shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    /*김찬수*/
    /*----------------------------------------------------------------------*/
    sf::Vector2f getSize() const {
        return shape.getSize();
    }

    // setPosition 메서드 추가
    void setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
        // 텍스트 위치 갱신
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
            shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }
    /*----------------------------------------------------------------------*/

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
