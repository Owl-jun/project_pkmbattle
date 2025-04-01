#pragma once
#include "BaseUI.hpp"
#include "UIButton.hpp"
#include "TimeManager.hpp"
#include <functional>
#include <vector>
#include <memory>

class FPSControl : public BaseUI {
private:
    sf::RectangleShape shape;
    sf::Text text;  // FPS 표시 텍스트
    sf::Font& font; // 폰트 참조
    std::vector<std::unique_ptr<UIButton>> buttons; // FPS 변경 버튼들
    sf::Color defaultColor;
    std::string label;

public:
    // FPSControl 생성자
    FPSControl(const std::string& label
        , const sf::Vector2f& position
        , const sf::Vector2f& size
        , sf::Color color
        , sf::Font& sharedFont
        , std::function<void(int)> fpsChangeFunc)
        : font(sharedFont)
        , label(label)
        , defaultColor(color)
        , text(sharedFont, label, 24)
    {
        // 배경 설정
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(color);

        // FPS 표시 텍스트 설정
        text.setFillColor(sf::Color::Black);
        text.setPosition({ shape.getPosition().x + shape.getSize().x / 2.f,
                           shape.getPosition().y + shape.getSize().y / 2.f });

        std::vector<int> fpsValues = { 10, 20, 30 };
        float offsetY = 2.f;

        for (int fps : fpsValues) {
            buttons.push_back(std::make_unique<UIButton>(
                sf::Vector2f(position.x, position.y),                // 버튼의 위치
                sf::Vector2f(100.f, 40.f),                           // 버튼의 크기
                "FPS: " + std::to_string(fps),                       // 버튼에 표시할 텍스트
                sf::Color(214, 181, 106, 255),                       // 버튼의 색상
                font,                                                // 버튼 폰트
                [this, fps]() {                                      // 버튼 클릭 람다
                    TimeManager::getInstance().setFPS(fps);          // FPS 변경
                    text.setString("FPS: " + std::to_string(fps));   // 버튼 클릭 후 텍스트 바뀐거(10,20...)
                },
                24                                                   // 버튼 텍스트 크기 설정
            ));
            offsetY += 50.f;                                         // 다음 버튼의 50만큼 아래
        }
    }

    sf::Vector2f getSize() const {
        return shape.getSize();
    }

    void setPosition(const sf::Vector2f& position) {
        // FPSControl의 배경 위치 설정
        shape.setPosition(position);
        text.setPosition({ position.x + shape.getSize().x / 2.f - text.getLocalBounds().size.x / 2.f,
                           position.y - 70.f });//

        // 버튼들의 위치를 FPSControl의 위치에 맞게 재조정
        float buttonOffsetY = 0.5f;  // 첫 번째 버튼 간격
        for (auto& button : buttons) {
            button->setPosition({ position.x + (shape.getSize().x - button->getSize().x) / 2.f,
                                  position.y + buttonOffsetY });
            buttonOffsetY += 50.f;  // 각 버튼 사이의 간격을 50으로 설정
        }
    }

    // 이벤트 처리 (버튼 클릭 감지)
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        for (auto& button : buttons) {
            button->handleEvent(event, window);
        }
    }

    // 업데이트 (필요 시 버튼 상태 갱신)
    void update(sf::RenderWindow& window) override {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(defaultColor);
        }
    }

    // 화면에 FPS 텍스트 및 버튼 렌더링
    void render(sf::RenderWindow& window) override {
        window.draw(text);
        window.draw(shape);
        for (auto& button : buttons) {
            button->render(window);  // 버튼 렌더링
        }
    }
};