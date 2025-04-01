//#pragma once
//#include "BaseUI.hpp"
//#include "UIButton.hpp"
//#include "TimeManager.hpp"
//#include <functional>
//#include <vector>
//#include <memory> // unique_ptr 사용을 위한 헤더 추가
//
//class FPSControl : public BaseUI {
//private:
//    sf::RectangleShape shape;
//    sf::Text text;  // FPS 표시 텍스트
//    std::function<void(int)> onFpsChange; // FPS 변경 콜백 함수
//    sf::Font& font; // 폰트 참조
//    std::vector<std::unique_ptr<UIButton>> buttons; // FPS 변경 버튼들
//    sf::Color defaultColor;
//
//public:
//    // FPSControl 생성자
//    FPSControl(const std::string& label
//        , const sf::Vector2f& position
//        , const sf::Vector2f& size
//        , sf::Color Color
//        , sf::Font& sharedFont
//        , std::function<void(int)> fpsChangeFunc)
//        : font(sharedFont)
//        , text(sharedFont, label, 48)
//        , defaultColor(Color) 
//    {
//        // 배경 설정
//        shape.setPosition(position);
//        shape.setSize(size);
//        shape.setFillColor(Color);
//
//        // FPS 표시 텍스트 설정
//        text.setFont(font);
//        text.setCharacterSize(24);
//        text.setFillColor(sf::Color::Black);
//        text.setString(label);
//        const auto bounds = text.getLocalBounds();  // 텍스트 크기를 가져옴
//        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
//        text.setPosition(
//            { shape.getPosition().x + shape.getSize().x / 2.f,
//            shape.getPosition().y + shape.getSize().y / 2.f }
//        );
//
//        // FPS 변경 버튼 생성 (각 버튼을 클릭하면 FPS 변경)
//        buttons.push_back(std::make_unique<UIButton>(
//            sf::Vector2f(position.x, position.y + 40.f),
//            sf::Vector2f(100.f, 40.f),
//            "FPS: 10",
//            sf::Color(214, 181, 106, 255),
//            font,
//            [this]() {
//                TimeManager::getInstance().setFPS(10);
//                text.setString("FPS: 10");
//            }
//        ));
//
//        buttons.push_back(std::make_unique<UIButton>(
//            sf::Vector2f(position.x, position.y + 90.f),
//            sf::Vector2f(100.f, 40.f),
//            "FPS: 20",
//            sf::Color(214, 181, 106, 255),
//            font,
//            [this]() {
//                TimeManager::getInstance().setFPS(20);
//                text.setString("FPS: 20");
//            }
//        ));
//
//        buttons.push_back(std::make_unique<UIButton>(
//            sf::Vector2f(position.x, position.y + 140.f),
//            sf::Vector2f(100.f, 40.f),
//            "FPS: 30",
//            sf::Color(214, 181, 106, 255),
//            font,
//            [this]() {
//                TimeManager::getInstance().setFPS(30);
//                text.setString("FPS: 30");
//            }
//        ));
//    }
//
//    // FPS 버튼 목록 반환
//    std::vector<std::unique_ptr<UIButton>>& getButtons() {
//        return buttons;
//    }
//
//    void setPosition(const sf::Vector2f& position) {
//        shape.setPosition(position);
//        // 텍스트 위치 갱신
//        text.setPosition(
//            { shape.getPosition().x + shape.getSize().x / 2.f,
//            shape.getPosition().y + shape.getSize().y / 2.f }
//        );
//    }
//
//    // 이벤트 처리 (버튼 클릭 감지)
//    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
//        for (auto& button : buttons) {
//            button->handleEvent(event, window);
//        }
//    }
//
//    // 업데이트 (필요 시 버튼 상태 갱신)
//    void update(sf::RenderWindow& window) override {
//        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
//        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
//        if (shape.getGlobalBounds().contains(mousePos)) {
//            shape.setFillColor(sf::Color::White);
//        }
//        else {
//            shape.setFillColor(defaultColor);
//        }
//    }
//
//    // 화면에 FPS 텍스트 및 버튼 렌더링
//    void render(sf::RenderWindow& window) override {
//        window.draw(text);
//        window.draw(shape);
//        for (auto& button : buttons) {
//            button->render(window);
//        }
//    }
//};

#pragma once
#include "BaseUI.hpp"
#include "UIButton.hpp"
#include "TimeManager.hpp"
#include <functional>
#include <vector>
#include <memory> // unique_ptr 사용을 위한 헤더 추가

class FPSControl : public BaseUI {
private:
    sf::RectangleShape shape;
    sf::Text text;  // FPS 표시 텍스트
    std::function<void(int)> onFpsChange; // FPS 변경 콜백 함수
    sf::Font& font; // 폰트 참조
    std::vector<std::unique_ptr<UIButton>> buttons; // FPS 변경 버튼들
    sf::Color defaultColor;

public:
    // FPSControl 생성자
    FPSControl(const std::string& label
        , const sf::Vector2f& position
        , const sf::Vector2f& size
        , sf::Color Color
        , sf::Font& sharedFont
        , std::function<void(int)> fpsChangeFunc)
        : font(sharedFont)
        , text(sharedFont, label, 48)
        , defaultColor(Color)
    {
        // 배경 설정
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(Color);

        // FPS 표시 텍스트 설정
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        const auto bounds = text.getLocalBounds();
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });  // 텍스트 중앙 정렬
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
            shape.getPosition().y + shape.getSize().y / 2.f }
        ); // 위치 조정

        // FPS 변경 버튼 생성 (각 버튼을 클릭하면 FPS 변경)
        buttons.push_back(std::make_unique<UIButton>(
            sf::Vector2f(position.x, position.y + 40.f),
            sf::Vector2f(100.f, 40.f),
            "FPS: 10",
            sf::Color(214, 181, 106, 255),
            font,
            []() {
                TimeManager::getInstance().setFPS(10);  // FPS 설정
            }
        ));

        buttons.push_back(std::make_unique<UIButton>(
            sf::Vector2f(position.x, position.y + 90.f),
            sf::Vector2f(100.f, 40.f),
            "FPS: 20",
            sf::Color(214, 181, 106, 255),
            font,
            []() {
                TimeManager::getInstance().setFPS(20);  // FPS 설정
            }
        ));

        buttons.push_back(std::make_unique<UIButton>(
            sf::Vector2f(position.x, position.y + 140.f),
            sf::Vector2f(100.f, 40.f),
            "FPS: 30",
            sf::Color(214, 181, 106, 255),
            font,
            []() {
                TimeManager::getInstance().setFPS(30);  // FPS 설정
            }
        ));
    }

    // FPS 버튼 목록 반환
    std::vector<std::unique_ptr<UIButton>>& getButtons() {
        return buttons;
    }

    void setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
        text.setPosition({ position.x + shape.getSize().x / 2.f, position.y - 30.f }); // 텍스트 위치 갱신
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