#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "KeyManager.hpp"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "TimeManager.hpp"
#include "SettingsOverlay.hpp"
#include "SelectOverlay.hpp"
#include "GameManager.hpp"

class UITextBox : public BaseUI {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    bool focused = false;
    std::wstring input;

    bool showCursor = true;
    float cursorTimer = 0.f;
    const float cursorBlinkInterval = 0.5f;
    sf::RectangleShape cursor;


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
        cursor.setSize({ 2.f, 24.f });
        cursor.setFillColor(sf::Color::Black);
        
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
                uint32_t unicode = textEntered->unicode;

                if (unicode == 8) { // backspace
                    if (!input.empty())
                        input.pop_back();
                }
                else if (unicode >= 32 && unicode != 127) {
                    input += static_cast<wchar_t>(unicode);
                }
            }
            updateText();
        }
    }
    void setPos(sf::Vector2f pos) {
        box.setPosition(pos);
        text.setPosition({ pos.x + 10.f, pos.y - 12.f });
    }
    void update(sf::RenderWindow& window) override {
        auto bounds = text.getGlobalBounds();
        float x = bounds.position.x + bounds.size.x + 2.f;
        float y = bounds.position.y;
        cursor.setPosition({ x, y });

        if (!focused) return;

        cursorTimer += TimeManager::getInstance().getDeltaTime();
        if (cursorTimer >= cursorBlinkInterval) {
            cursorTimer = 0.f;
            showCursor = !showCursor;
        }


    }

    void render(sf::RenderWindow& window) override {
        window.draw(box);
        window.draw(text);
        if (focused && showCursor)
            window.draw(cursor);
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

    void updateText() {
        std::wcout << L"[DEBUG] input : " << input << std::endl;
        sf::String s;
        for (wchar_t ch : input) {
            s += ch; 
        }
        text.setString(s);
    }

    std::string wstringToUtf8(const std::wstring& wstr) const {
        std::string result;
        for (wchar_t wc : wstr) {
            if (wc <= 0x7F) {
                result.push_back(static_cast<char>(wc));
            }
            else if (wc <= 0x7FF) {
                result.push_back(static_cast<char>(0xC0 | ((wc >> 6) & 0x1F)));
                result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
            }
            else if (wc <= 0xFFFF) {
                result.push_back(static_cast<char>(0xE0 | ((wc >> 12) & 0x0F)));
                result.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
            }
            else {
                result.push_back(static_cast<char>(0xF0 | ((wc >> 18) & 0x07)));
                result.push_back(static_cast<char>(0x80 | ((wc >> 12) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
            }
        }
        return result;
    }

    std::string getInput() const {
        return wstringToUtf8(input);
    }


};
