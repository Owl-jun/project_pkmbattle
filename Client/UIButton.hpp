#pragma once
#include "BaseUI.hpp"
#include "KeyManager.h"
#include <functional>

class UIButton : public BaseUI {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;
    sf::Color defaultColor;

public:

    // 🔹 기존 string 생성자 유지
    UIButton(const sf::Vector2f& pos,
        const sf::Vector2f& size,
        const std::string& label,
        sf::Color Color,
        sf::Font& sharedFont,
        std::function<void()> clickFunc)
        : onClick(clickFunc)
        , text(sharedFont, label, 48)
        , defaultColor(Color)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Color);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::Black);

        const auto bounds = text.getLocalBounds();
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
              shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    // 🔸 추가: wstring 생성자 (한글 깨짐 방지용)
    UIButton(const sf::Vector2f& pos,
        const sf::Vector2f& size,
        const std::wstring& label,
        sf::Color Color,
        sf::Font& sharedFont,
        std::function<void()> clickFunc)
        : onClick(clickFunc)
        , text(sharedFont, label, 30)
        , defaultColor(Color)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Color);

        text.setFont(sharedFont);
        text.setString(label); // 와이드 문자열
        text.setCharacterSize(36); //텍스트 UI 폰트 크기!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        text.setFillColor(sf::Color::Black);

        const auto bounds = text.getLocalBounds();
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });        
        text.setPosition(
         {shape.getPosition().x + shape.getSize().x / 2.f, shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    sf::Vector2f getSize() const {
        return shape.getSize();
    }

    void setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
        const auto bounds = text.getLocalBounds();
        text.setPosition(                                       // fight, run 버튼 텍스 위치
            { shape.getPosition().x + shape.getSize().x / 1.6f - bounds.size.x / 1.6f,
              shape.getPosition().y + shape.getSize().y / 1.6f - bounds.size.y / 1.6f }
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

    void click() {
        if (onClick) onClick();
    }

    bool isFocusable() const override { return true; }
    bool isFocused() const override { return shape.getOutlineColor() == sf::Color::Yellow; }

    void setFocus(bool focus) override {
        if (focus)
            shape.setOutlineColor(sf::Color::Yellow);
        else
            shape.setOutlineColor(sf::Color::Transparent);
        shape.setOutlineThickness(focus ? 3.f : 0.f);
    }
};
