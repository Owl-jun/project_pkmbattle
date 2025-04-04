#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "UIButton.hpp"
#include "KeyManager.hpp"
#include "GameManager.hpp"
#include "UIManager.hpp"
#include "ResourceManager.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"

#include <functional>
#include <memory>
#include <vector>
#include <iostream> // 디버깅 메시지를 위해 추가
#include <stdexcept> // 예외 처리를 위해 추가

class SettingsOverlay {
private:
    sf::RenderWindow& window;
    sf::RectangleShape background;
    sf::Text title;
    sf::Font font;
    std::vector<std::unique_ptr<BaseUI>> uiElements;
    bool visible = false;

    // Yes와 No 버튼의 인덱스를 저장할 변수
    int yesButtonIndex = -1;
    int noButtonIndex = -1;
    int exitButtonIndex = -1;
    int backButtonIndex = -1;

    AnimationManager aniManager;
    sf::Sprite back = sf::Sprite(ResourceManager::getInstance().getTextureByName("settingsback.png"));
public:
    SettingsOverlay(const sf::Vector2f& size, const sf::Font& sharedFont)
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF")), title(font, "Settings", 36), window(GameManager::getInstance().getWindow())
    {
        background.setSize(size);
        background.setFillColor(sf::Color(50, 50, 50, 180));
        background.setPosition({ 0.f, 0.f });

        title.setFillColor(sf::Color::Black);
        title.setPosition({ 30.f, 20.f });

        back.setPosition({ background.getPosition().x + 400.f, background.getPosition().y + 300.f });

        // "Back" 이미지 버튼 생성
        auto backButton = std::make_unique<UIButton>(
            sf::Vector2f(0.f, 0.f), // 버튼 위치
            sf::Vector2f(30.f, 30.f), // 버튼 크기
            "",
            24, // 텍스트 크기
            sf::Color(214, 181, 106, 50), // 버튼 색상
            [this]() { // 클릭 시 실행될 함수
                toggle(); // 세팅 화면을 닫기
            }
        );
        backButton->setHoverEffect(true);
        // addElement로 "Back" 이미지 버튼 추가
        addElement(std::move(backButton));
        backButtonIndex = uiElements.size() - 1;

        // "Exit" 버튼 생성
        auto exitButton = std::make_unique<UIButton>(
            sf::Vector2f(0.f, 0.f), // 버튼 위치
            sf::Vector2f(120.f, 40.f), // 버튼 크기
            "EXIT", // 버튼 텍스트
            24, // 텍스트 크기
            sf::Color(214, 181, 106, 255), // 버튼 색상
            [this]() { // 클릭 시 실행될 함수
                if (yesButtonIndex == -1 && noButtonIndex == -1) {
                    std::cout << "나가기 버튼 눌림" << std::endl;
                    // "Yes"와 "No" 버튼 표시
                    showYesNoButtons();
                }
            }
        );

        // addElement로 "Exit" 버튼 추가
        addElement(std::move(exitButton));
        exitButtonIndex = uiElements.size() - 1;
    }

    void toggle() {
        visible = !visible;
    }

    bool isVisible() const {
        return visible;
    }

    void setCenter(const sf::Vector2f& center) {
        background.setPosition({ center - (background.getSize() / 2.f) });
        title.setPosition({ center.x - title.getLocalBounds().size.x / 2.f, center.y - background.getSize().y / 2.f + 20.f });

        // 버튼 위치를 설정
        for (size_t i = 0; i < uiElements.size(); ++i) {
            if (auto* button = dynamic_cast<UIButton*>(uiElements[i].get())) {
                if (i == yesButtonIndex) {
                    // "Yes" 버튼의 위치 설정
                    button->setPosition({
                        center.x - 130.f,
                        center.y + 10.f
                        });
                }
                else if (i == noButtonIndex) {
                    // "No" 버튼의 위치 설정
                    button->setPosition({
                        center.x + 10.f,
                        center.y + 10.f
                        });
                }
                else if (i == exitButtonIndex) {
                    // "Exit" 버튼의 위치 설정
                    button->setPosition({
                        center.x - button->getSize().x / 2.f ,
                        center.y - button->getSize().y / 2.f + 200.f
                        });
                }
                else if (i == backButtonIndex) {
                    // "Back" 이미지 버튼의 위치 설정
                    button->setPosition({
                        background.getPosition().x + 30.f,
                        background.getPosition().y + 20.f
                        });
                }
            }
        }
        back.setPosition({ background.getPosition().x + 30.f, background.getPosition().y + 20.f });
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) {
        if (!visible) return;
        for (auto& ui : uiElements)
            ui->handleInput(event, window);
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
        window.draw(back);
    }

    void addElement(std::unique_ptr<BaseUI> ui) {
        uiElements.push_back(std::move(ui));
    }

    // "Yes"와 "No" 버튼을 표시하는 함수
    void showYesNoButtons() {
        if (yesButtonIndex == -1 && noButtonIndex == -1) { // "Yes"와 "No" 버튼이 이미 있는 경우 중복 추가 방지
            // "Yes" 버튼 생성
            auto yesButton = std::make_unique<UIButton>(
                sf::Vector2f(0.f, 0.f), // 버튼 위치
                sf::Vector2f(120.f, 40.f), // 버튼 크기
                "Yes", // 버튼 텍스트
                24, // 텍스트 크기
                sf::Color(100, 200, 100, 255), // 버튼 색상
                [this]() { // 클릭 시 실행될 함수
                    std::cout << "Yes 버튼 눌림" << std::endl;
                    NetworkManager::getInstance().send("EXIT\n");
                }
            );

            // "No" 버튼 생성
            auto noButton = std::make_unique<UIButton>(
                sf::Vector2f(0.f, 0.f), // 버튼 위치
                sf::Vector2f(120.f, 40.f), // 버튼 크기
                "No", // 버튼 텍스트
                24, // 텍스트 크기
                sf::Color(200, 100, 100, 255), // 버튼 색상
                [this]() { // 클릭 시 실행될 함수
                    std::cout << "No 버튼 눌림" << std::endl;
                    hideYesNoButtons(); // "Yes"와 "No" 버튼 숨기기
                }
            );

            // addElement로 "Yes"와 "No" 버튼 추가하고 인덱스 저장
            addElement(std::move(yesButton));
            yesButtonIndex = uiElements.size() - 1; // 인덱스 저장

            addElement(std::move(noButton));
            noButtonIndex = uiElements.size() - 1; // 인덱스 저장

            // "Yes"와 "No" 버튼의 위치 설정
            setCenter({ window.getSize().x / 2.f, window.getSize().y / 2.f });
        }
    }

    // "Yes"와 "No" 버튼을 숨기는 함수
    void hideYesNoButtons() {
        if (noButtonIndex != -1 && noButtonIndex < uiElements.size()) {
            uiElements.erase(uiElements.begin() + noButtonIndex);
            noButtonIndex = -1; // 인덱스 초기화
        }

        if (yesButtonIndex != -1 && yesButtonIndex < uiElements.size()) {
            uiElements.erase(uiElements.begin() + yesButtonIndex);
            yesButtonIndex = -1; // 인덱스 초기화
        }
    }
};