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
    std::vector<BaseUI*> originalButtons;
    bool isWaiting = false; //
    sf::Font font;
    UIManager uiManager;
    bool visible = false;
    int currentFocusIndex = 0;
    bool enterPressed = false;
    std::function<void()> onFightCallback; // 🔹 싸운다 콜백
    std::function<void()> onRunCallback; // 🔹 싸운다 콜백
    float escCooldown = 0.f;


public:
    SelectOverlay(const sf::Vector2f& size, const sf::Font& sharedFont)
        : font(sharedFont), title(font, L"누군가 싸움을 걸었다!", 38) {
       
        background.setSize(size);
        background.setFillColor(sf::Color(0, 0, 0, 0));
        background.setPosition({ 0.f, 0.f });

        title.setFillColor(sf::Color::Yellow);
        title.setPosition({ 30.f, 20.f });

        std::wstring L8 = L"싸우자!!";
        std::wstring L9 = L"도망간다!";

        auto fightButton = new UIButton({ 8.f, 15.f }, { 180.f, 40.f }, L8, sf::Color::White, font, [this]() {
            std::cout << "[선택됨] 싸운다!" << std::endl;
            if (onFightCallback) onFightCallback(); // 🔹 콜백 호출
            });

        auto runButton = new UIButton({ 20.f, 55.f }, { 180.f, 40.f }, L9, sf::Color::White, font, [this]() {
            std::cout << "[선택됨] 도망간다!" << std::endl;
            if (onRunCallback) onRunCallback(); // 🔹 콜백 호출
            visible = false;
            });

        originalButtons = { fightButton, runButton };

        for (auto* btn : originalButtons) {
            uiManager.addElement(btn);
        }
        // 최초 포커스 설정
        fightButton->setFocus(true);
    }
    // 🔹 콜백 세터
    void setFightCallback(std::function<void()> cb) { onFightCallback = cb; }
    void setRunCallback(std::function<void()> cb) { onRunCallback = cb; }

    void setMessage(const std::wstring& msg) {
        title.setString(msg);
    }

    void setWaiting(bool waiting) {
        isWaiting = waiting;
        uiManager.removeAllElements();

        if (!waiting) {
            for (auto* btn : originalButtons)
                uiManager.addElement(btn); // 다시 추가
            originalButtons[0]->setFocus(true);
            currentFocusIndex = 0;
        }
    }
    void toggle() { visible = true; }
    void hide() { visible = false; }
    bool isVisible() const { return visible; }

    void setCenter(const sf::Vector2f& center) {
        background.setPosition(center - background.getSize() / 20.f);
        // SetPosition -> center 기준으로 전체 박스를 가운데 배치하는 거고,
        title.setPosition({ center.x - title.getLocalBounds().size.x / 2.f, center.y - background.getSize().y / 2.f + 20.f });
        for (auto& ui : uiManager.getElements()) {
            if (auto* button = dynamic_cast<UIButton*>(ui)) {
                // 이건 title을 박스 안 위쪽 중앙에 두는 거고,
                button->setPosition({
                    background.getPosition().x + background.getSize().x / 1.3f - title.getLocalBounds().size.x / 2.f,
                    center.y - background.getSize().y + 300.f + (button->getSize().y + 10.f) * uiManager.getElementIndex(ui)
                    });
            }
        }
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (!visible || isWaiting) return;
        // Enter/Tab 등은 SelectOverlay에서만 처리하고, UIManager에는 전달하지 않음
        // uiManager.handleEvent(event, window); ← 제거

        auto& elements = uiManager.getElements();
        int size = static_cast<int>(elements.size());
        if (size == 0) return;

        if (event.is<sf::Event::KeyPressed>()) {
            auto key = event.getIf<sf::Event::KeyPressed>();

            if (key->code == sf::Keyboard::Key::Up) {
                elements[currentFocusIndex]->setFocus(false);
                if (currentFocusIndex > 0) { currentFocusIndex = (currentFocusIndex - 1) % size; }
                else { currentFocusIndex = (currentFocusIndex + 1) % size; }
                elements[currentFocusIndex]->setFocus(true);
            }
            else if (key->code == sf::Keyboard::Key::Down) {
                elements[currentFocusIndex]->setFocus(false);
                currentFocusIndex = (currentFocusIndex + 1) % size;
                elements[currentFocusIndex]->setFocus(true);
            }
            else if (key->code == sf::Keyboard::Key::Space && !enterPressed && escCooldown <= 0.f) {
                escCooldown = 0.5f;
                enterPressed = true;
                if (auto* button = dynamic_cast<UIButton*>(elements[currentFocusIndex])) {
                    button->click();
                }
            }
        }
        else if (event.is<sf::Event::KeyReleased>()) {
            auto key = event.getIf<sf::Event::KeyReleased>();
            if (key->code == sf::Keyboard::Key::Space) {
                enterPressed = false;
            }
        }
    }

    void disableCallbacks() {
        onFightCallback = []() {};
        onRunCallback = []() {};
    }

    void show() {
        if (!visible) visible = true;
    }

    void update(sf::RenderWindow& window) {
        if (!visible) return;
        escCooldown -= TimeManager::getInstance().getDeltaTime();
        uiManager.update(window);
    }

    void render(sf::RenderWindow& window) {
        if (!visible) return;
        window.draw(background);
        window.draw(title);
        if (!isWaiting)
            uiManager.render(window);
    }

};
