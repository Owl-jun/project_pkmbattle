#pragma once

#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
#include "LoginScene.hpp"
#include "worldScene.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"
#include "UIButton.hpp"
#include "UITextBox.hpp"
#include "UIManager.hpp"
#include "Player.h"
#include "ResourceManager.hpp"
#include "NetworkManager.hpp"
#include "GameManager.h"

class Testzz : public BaseScene {
public:
    Testzz() {}

    void init() override {
        // 초기화 로직 없음
        std::cout << "[TestScene] init() 호출됨\n";
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        // 서버연결없이 테스트용
        if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::F5)) {
            SceneManager::getInstance().changeScene("world");
        }
 /*       if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Num1) && selectCooldown <= 0.f) {
            selectOverlay.toggle();
            selectOverlay.setCenter(camera.getCenter());
            std::cout << "SelectOverlay Visible: " << selectOverlay.isVisible() << "\n";
            selectCooldown = 0.5f;
        }*/
    }

    void update(sf::RenderWindow& window) override {
        // 아무 업데이트도 안 함
    }

    void render(sf::RenderWindow& window) override {
        // 그냥 하얀 배경으로 클리어만
        window.clear(sf::Color::White);
        // 추가 렌더 없음
    }
};
