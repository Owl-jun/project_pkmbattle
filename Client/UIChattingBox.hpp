#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "UIButton.hpp"

class UIChattingBox : public BaseUI {
private:
    sf::RectangleShape background;
    sf::Font font = ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF");
    std::vector<sf::Text> messages;
    size_t max_line = 10;
    int curIndex = 0;

    UIButton toggleBtn;
    bool isVisible = true;

public:
    UIChattingBox()
        : toggleBtn({0.f,0.f},{50.f,20.f},"CLOSE",18,sf::Color({0,0,0,50}), [&]() {
        isVisible = !isVisible;
            })
    {
        background.setSize({ 800.f,240.f });
        background.setFillColor({ 0, 0, 0, 50 });
        background.setPosition({ 0.f,360.f });
        
        for (int i = 0; i < max_line; ++i)
        {
            messages.push_back(sf::Text(font, L"", 24));
        }
    }
    
    void setPos(sf::Vector2f pos) {
        background.setPosition(pos);
        toggleBtn.setPosition(
            { background.getPosition().x + background.getSize().x - 100.f
            , background.getPosition().y });
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse &&
                mouse->button == sf::Mouse::Button::Left &&
                toggleBtn.getShape().getGlobalBounds().contains(mousePos)) {
                toggleBtn.click(); 
            }
        }
        if (event.is<sf::Event::KeyPressed>()) {
            if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::F12)) {
                toggleBtn.click();
            }
        }
    }
    void update(sf::RenderWindow& window) {
        toggleBtn.update(window);
    }
    void render(sf::RenderWindow& window) {
        if (isVisible)
        {
            window.draw(background);
            toggleBtn.setString("CLOSE (F12)");
            for (size_t i = 0; i < max_line; ++i) {
                // 최신 메시지부터 위로 올라가게
                int index = (curIndex - 1 - i + max_line) % max_line;
                if (messages[index].getString().isEmpty()) continue; // 빈 메시지 건너뛰기

                // 위치를 아래쪽부터 위쪽으로 배치
                sf::Vector2f bgPos = background.getPosition();
                messages[index].setPosition({ bgPos.x + 10.f, bgPos.y + background.getSize().y - 40.f - i * 22.f });

                window.draw(messages[index]);
            }
        }
        else {
            toggleBtn.setString("OPEN (F12)");
        }
        toggleBtn.render(window);
    }

    void addMessage(const std::string& msg) {
        std::cout << "[디버그용 메시지] msg: " << msg << std::endl;
        sf::String s = msg;
        messages[curIndex % max_line].setString(s.fromUtf8(s.begin(),s.end()));
        curIndex++;
    }

};