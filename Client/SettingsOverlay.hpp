#pragma once
#include "pch.h"
#include "TimeManager.hpp"
#include "BaseUI.hpp"
#include "UISlider.hpp"
#include "KeyManager.hpp"
#include "UImuteControl.hpp"
#include "FPSControl.hpp"
#include <functional>
#include "SoundManager.hpp"
#include "GameManager.hpp"
#include <memory>

class SettingsOverlay {
private:
    sf::RenderWindow& window;
    sf::RectangleShape background;
    sf::Text title;
    sf::Font font;
    std::vector<std::unique_ptr<BaseUI>> uiElements;
    bool visible = false;

    /* 초기 볼륨 */
    float volume = 50.f;

public:
    SettingsOverlay(const sf::Vector2f& size, const sf::Font& sharedFont)
        : font(sharedFont), title(font, "Settings", 36), window(GameManager::getInstance().getWindow())
    {
        background.setSize(size);
        background.setFillColor(sf::Color(50, 50, 50, 180));
        background.setPosition({ 0.f, 0.f });

        title.setFillColor(sf::Color::Black);
        title.setPosition({ 30.f, 20.f });

        /*----------------------------------------------------------------------*/
        // 나가기 버튼
        auto exitButton = std::make_unique<UIButton>(
            sf::Vector2f(0.f, 0.f), // 버튼 위치
            sf::Vector2f(120.f, 40.f), // 버튼 크기
            "Exit", // 버튼 텍스트
            sf::Color(214, 181, 106, 255), // 버튼 색상
            font, // 폰트
            [this]() { // 클릭 시 실행될 함수
                std::cout << "나가기 버튼 눌림" << std::endl;
                window.close(); // 게임 닫기
            }
        );

        // 볼륨 슬라이더 버튼
        auto volumeSlider = std::make_unique<UISlider>(
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(150.f, 10.f),
            &this->volume
        );

        // 볼륨 뮤트 버튼
        auto muteControl = std::make_unique<UImuteControl>(sf::Vector2f(720, 10.f));

        /* FPS 설정 버튼 추가 */
        auto fps10 = std::make_unique<FPSControl>(
            "",
            sf::Vector2f(100.f, 50.f),
            sf::Vector2f(1.f, 1.f),
            sf::Color(100, 100, 100, 255),
            font,
            [](int fps) { TimeManager::getInstance().setFPS(fps); }
        );

        auto fps20 = std::make_unique<FPSControl>(
            "",
            sf::Vector2f(100.f, 100.f),
            sf::Vector2f(1.f, 1.f),
            sf::Color(100, 100, 100, 255),
            font,
            [](int fps) { TimeManager::getInstance().setFPS(fps); }
        );

        auto fps30 = std::make_unique<FPSControl>(
            "FPS 30",
            sf::Vector2f(100.f, 150.f),
            sf::Vector2f(1.f, 1.f),
            sf::Color(100, 100, 100, 255),
            font,
            [](int fps) { TimeManager::getInstance().setFPS(fps); }
        );

        // addElement로 보내기
        addElement(std::move(exitButton));
        addElement(std::move(muteControl));
        addElement(std::move(volumeSlider));
        addElement(std::move(fps10));
        addElement(std::move(fps20));
        addElement(std::move(fps30));

        // 노래 파일 위치
        if (!SoundManager::getInstance().playMusic("C:\\Source\\project_pkmbattle\\Client\\assets\\track1.mp3")) {
            std::cerr << "음악 파일 로드 실패!" << std::endl;
        }

        // 초기 볼륨 설정
        SoundManager::getInstance().setVolume(true);
        /*----------------------------------------------------------------------*/

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

        float offsetY = center.y - background.getSize().y / 2.f + 80.f;

        for (auto& ui : uiElements) {
            // exit 버튼 좌표
            if (auto* exit = dynamic_cast<UIButton*>(ui.get())) {
                exit->setPosition({
                    center.x + background.getSize().x / 2.f - exit->getSize().x - 10.f,
                    center.y + background.getSize().y / 2.f - exit->getSize().y - 10.f
                    });
            }
            // mute 버튼 좌표
            else if (auto* mute = dynamic_cast<UImuteControl*>(ui.get())) {
                mute->setPosition({
                    center.x + background.getSize().x / 2.f - 57.f,
                    center.y - background.getSize().y / 2.f - 10.f
                    });
            }
            // volume slider 좌표
            else if (auto* slider = dynamic_cast<UISlider*>(ui.get())) {
                slider->setPosition({
                    center.x + background.getSize().x / 2.f - 43.f,
                    center.y - background.getSize().y / 2.f + 90.f
                    });
            }

            // FPS 버튼
            for (auto& ui : uiElements) {
                if (auto* fps = dynamic_cast<FPSControl*>(ui.get())) {
                    float offsetX = (background.getSize().x - fps->getSize().x) / 2.f;  // 중앙 정렬
                    float offsetY = uiElements.size() * 25.f;  // 아래로 간격을 두고 배치
                    fps->setPosition({ center.x - background.getSize().x / 2.f + offsetX, center.y - background.getSize().y / 2.f + offsetY });
                }
            }
        }
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (!visible) return;
        for (auto& ui : uiElements)
            ui->handleEvent(event, window);

        SoundManager::getInstance().setVolume(volume >= SoundManager::getInstance().getVolume());
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

    void addElement(std::unique_ptr<BaseUI> ui) {
        uiElements.push_back(std::move(ui));
    }
};