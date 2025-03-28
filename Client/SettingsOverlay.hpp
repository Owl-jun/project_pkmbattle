#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "KeyManager.h"
#include "UImuteControl.hpp"
#include <functional>

class SettingsOverlay {
private:
    sf::RectangleShape background;
    sf::Text title;
    sf::Font font;
    std::vector<std::unique_ptr<BaseUI>> uiElements;
    bool visible = false;

    /*김찬수*/
    /*----------------------------------------------------------------------*/
    float volume = 50.f;
    /*----------------------------------------------------------------------*/

public:
    SettingsOverlay(const sf::Vector2f& size, const sf::Font& sharedFont)
        : font(sharedFont), title(font, "Settings", 36)
    {
        background.setSize(size);
        background.setFillColor(sf::Color(50, 50, 50, 180));
        background.setPosition({ 0.f, 0.f });

        title.setFillColor(sf::Color::Black);
        title.setPosition({ 30.f, 20.f });

        // 예시: 볼륨 조절, 나가기 버튼 등 추가 가능
        // 이곳에 UIButton 등 UI 추가 가능
        /*김찬수*/
        /*----------------------------------------------------------------------*/
        auto exitButton = new UIButton(
            {  0.f, 0.f },   // 버튼 위치
            { 120.f, 40.f },                     // 버튼 크기
            "Exit",                              // 버튼 텍스트
            sf::Color(214, 181, 106, 255),                    // 버튼 색상
            font,                                // 폰트
            [this]() {                           // 클릭 시 실행될 함수
                std::cout << "나가기 버튼 눌림" << std::endl;
                this->toggle();                  // SettingsOverlay 닫기
            }
        );
        /*----------------------------------------------------------------------*/
        auto muteControl = new UImuteControl({ 720,10.f });
        addElement(exitButton);
        addElement(std::move(muteControl));
    }

    void toggle() {
        visible = !visible;
    }

    bool isVisible() const {
        return visible;
    }

    void setCenter(const sf::Vector2f& center) {
        background.setPosition({ center - background.getSize() / 2.f });
        title.setPosition({ center.x - title.getLocalBounds().size.x / 2.f, center.y - background.getSize().y / 2.f + 20.f });

        for (auto& ui : uiElements) {
            if (auto* exit = dynamic_cast<UIButton*>(ui.get())) {
                exit->setPosition({
                    center.x + background.getSize().x / 2.f - exit->getSize().x - 10.f,
                    center.y + background.getSize().y / 2.f - exit->getSize().y - 10.f
                    });
            }

            else if (auto* mute = dynamic_cast<UImuteControl*>(ui.get())) {
                mute->setPosition({
                    center.x + background.getSize().x / 2.f - 60.f,  // 아이콘 너비 고려해서 오른쪽 끝에서 살짝 안쪽
                    center.y - background.getSize().y / 2.f  
                    });
            }
        }
    }



    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (!visible) return;
        for (auto& ui : uiElements)
            ui->handleEvent(event, window);
    }

    void update(sf::RenderWindow& window) {
        if (!visible) return;
        for (auto& ui : uiElements)
            ui->update(window);
    }

    void render(sf::RenderWindow& window) {
        if (!visible) return;
        window.draw(background);
        window.draw(title);
        for (auto& ui : uiElements)
            ui->render(window);
    }

    void addElement(BaseUI* ui) {
        uiElements.emplace_back(std::move(ui));
    }

    void clear() {
        uiElements.clear();
    }
};