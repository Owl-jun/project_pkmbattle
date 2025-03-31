// 보정 동관
// UI BOX 만들기 (싸운다, 도망간다) {배틀씬 들어가기전 텍스트 UI}

#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include <windows.h>
#include <functional>

class SelectOverlay {
private:
    sf::RectangleShape background;
    sf::Text title;
    sf::Font font;
    UIManager uiManager;
    bool visible = false;
    int currentFocusIndex = 0;
    bool enterPressed = false;

public:
    SelectOverlay(const sf::Vector2f& size, const sf::Font& sharedFont)
        : font(sharedFont), title(font, "Select Action", 48) {
       
        background.setSize(size);
        background.setFillColor(sf::Color(50, 50, 50, 180));
        background.setPosition({ 0.f, 0.f });

        title.setFillColor(sf::Color::Black);
        title.setPosition({ 30.f, 20.f });

        std::wstring L8 = L"싸우자!!";
        std::wstring L9 = L"너 밥이야";

        auto fightButton = new UIButton({ 8.f, 15.f }, { 180.f, 30.f }, L8, sf::Color::White, font, [this]() {
            std::cout << "[선택됨] 싸운다!" << std::endl;
            });

        auto runButton = new UIButton({ 20.f, 55.f }, { 180.f, 30.f }, L9, sf::Color::White, font, [this]() {
            std::cout << "[선택됨] 도망간다!" << std::endl;
            });
        uiManager.addElement(fightButton);
        uiManager.addElement(runButton);

        // 최초 포커스 설정
        fightButton->setFocus(true);
    }

    void toggle() { visible = !visible; }
    void hide() { visible = false; }
    bool isVisible() const { return visible; }

    void setCenter(const sf::Vector2f& center) {
        background.setPosition(center - background.getSize() / 2.f);
        title.setPosition({ center.x - title.getLocalBounds().size.x / 2.f, center.y - background.getSize().y / 2.f + 20.f });

        for (auto& ui : uiManager.getElements()) {
            if (auto* button = dynamic_cast<UIButton*>(ui)) {
                button->setPosition({
                    center.x - button->getSize().x / 2.f,
                    center.y - background.getSize().y / 2.f + 60.f + (button->getSize().y + 10.f) * uiManager.getElementIndex(ui)
                    });
            }
        }
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (!visible) return;
        // Enter/Tab 등은 SelectOverlay에서만 처리하고, UIManager에는 전달하지 않음
        // uiManager.handleEvent(event, window); ← 제거

        auto& elements = uiManager.getElements();
        int size = static_cast<int>(elements.size());
        if (size == 0) return;

        if (event.is<sf::Event::KeyPressed>()) {
            auto key = event.getIf<sf::Event::KeyPressed>();

            if (key->code == sf::Keyboard::Key::Tab) {
                elements[currentFocusIndex]->setFocus(false);
                currentFocusIndex = (currentFocusIndex + 1) % size;
                elements[currentFocusIndex]->setFocus(true);
            }
            else if (key->code == sf::Keyboard::Key::Enter && !enterPressed) {
                enterPressed = true;
                if (auto* button = dynamic_cast<UIButton*>(elements[currentFocusIndex])) {
                    button->click();
                }
            }
        }
        else if (event.is<sf::Event::KeyReleased>()) {
            auto key = event.getIf<sf::Event::KeyReleased>();
            if (key->code == sf::Keyboard::Key::Enter) {
                enterPressed = false;
            }
        }
    }

    void update(sf::RenderWindow& window) {
        if (!visible) return;
        uiManager.update(window);
    }

    void render(sf::RenderWindow& window) {
        if (!visible) return;
        window.draw(background);
        window.draw(title);
        uiManager.render(window);
    }
};
