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
                    // ASCII ���ڸ� ó��
                    if (textEntered->unicode != 9)
                    {
                        input += static_cast<char>(textEntered->unicode);
                    }
                    
                }
                text.setString(input);
            }
        }
    }

    void update(sf::RenderWindow& window) override {
        // blinking Ŀ�� �� ���� �� ����
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

    bool isFocusable() const override { return true; }
    bool isFocused() const override { return focused; }

    void setFocus(bool focus) override { 
        focused = focus; 
        if (focused)
            box.setFillColor(sf::Color::Yellow); 
        else
            box.setFillColor(sf::Color::White);
    }

};
