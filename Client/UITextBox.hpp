#pragma once
#include "BaseUI.hpp"

class UITextBox : public BaseUI {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    bool focused = false;
    std::string input;

public:
    UITextBox(const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& sharedFont)
        : font(sharedFont), text(font, "", 24)
    {
        box.setPosition(pos);
        box.setSize(size);
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2.f);

        text.setPosition({ pos.x + 10.f, pos.y -12.f });
        text.setFillColor(sf::Color::Black);
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        if (event.is<sf::Event::MouseButtonPressed>()) {
            auto mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse) {
                sf::Vector2f mousePos = window.mapPixelToCoords({ mouse->position.x, mouse->position.y });
                focused = box.getGlobalBounds().contains(mousePos);
            }
        }

        if (!focused) return;

        if (event.is<sf::Event::TextEntered>()) {
            auto textEntered = event.getIf<sf::Event::TextEntered>();
            if (textEntered) {
                if (textEntered->unicode == '\b') {
                    // Backspace
                    if (!input.empty())
                        input.pop_back();
                }
                else if (textEntered->unicode < 128) {
                    // ASCII 문자만 처리
                    input += static_cast<char>(textEntered->unicode);
                }
                text.setString(input);
            }
        }
    }

    void update(sf::RenderWindow& window) override {
        // blinking 커서 등 넣을 수 있음
    }

    void render(sf::RenderWindow& window) override {
        window.draw(box);
        window.draw(text);
    }

    std::string getInput() const {
        return input;
    }

    void clear() {
        input.clear();
        text.setString("");
    }

    void setFocus(bool value) {
        focused = value;
    }

    bool isFocused() const {
        return focused;
    }
};
