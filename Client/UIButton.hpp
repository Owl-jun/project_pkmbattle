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
        , text(sharedFont,label,24)
        , defaultColor(Color)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Color);

        //font.openFromFile("C:/Windows/Fonts/arial.ttf"); 
        //text.setFont(font);
        //text.setString(label);
        //text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        const auto bounds = text.getLocalBounds();  // sf::FloatRect
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
            shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        std::cout << "[DEBUG] handleEvent 호출됨\n";

        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse && mouse->button == sf::Mouse::Button::Left) {
                std::cout << "씨발새끼야" << std::endl;
                SceneManager::getInstance().changeScene(new LoginScene());
            }
        }
    }



    void update(sf::RenderWindow& window) override {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);  // 🧠 여기 중요! window 기준으로 가져옴
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setFillColor(sf::Color::Cyan); // hover 색상
        }
        else {
            shape.setFillColor(defaultColor); // 원래 색상
        }
    }

    void render(sf::RenderWindow& window) override {
        window.draw(shape);
        window.draw(text);
    }
};
