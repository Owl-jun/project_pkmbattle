#pragma once

#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.h"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"

class testScene : public BaseScene {
private:
    sf::Font font;
    UIManager uiManager;
    AnimationManager aniManager;
    sf::Texture backgroundTex;  // 배경 텍스처
    std::optional<sf::Sprite> background;  // 배경 스프라이트
    sf::Vector2f bgtextureSize;  // 배경 텍스처 크기
    sf::Vector2f windowSize;  // 윈도우 크기

    bool firstImageDone = false;
    bool secondImageAdded = false;  // 두 번째 이미지가 추가되었는지 확인
    float timer = 0.f;
    float timeToWait = 2.f;  // 첫 번째 이미지가 나타날 시간 (2초로 설정)

public:
    testScene()
        : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.ttf")
    {
    }

    void init() override {
        // 배경 이미지 로드
        if (!backgroundTex.loadFromFile("C:/Source/project_pkmbattle/Client/assets/introbg.png")) {
            std::cerr << "Failed to load background texture!" << std::endl;
        }

        // 배경 이미지 크기 맞추기
        bgtextureSize = static_cast<sf::Vector2f>(backgroundTex.getSize());
        windowSize = static_cast<sf::Vector2f>(GameManager::getInstance().getWindow().getSize());

        background.emplace(backgroundTex);
        background->setScale({ (windowSize.x / bgtextureSize.x), (windowSize.y / bgtextureSize.y) });
        background->setPosition({ 0.f, 0.f });

        // 첫 번째 이미지
        AnimatedObject* image1 = new AnimatedObject(
            "C:/Source/project_pkmbattle/Client/assets/introbg.png",
            { 0, 0 }, 200.f, 1.0f
        );

        aniManager.add(*image1, [this, image1](AnimatedObject& obj, float dt) {
            obj.intro(dt);
            timer += dt;

            // 일정 시간 (2초) 후 첫 번째 이미지가 사라지고 두 번째 이미지로 교체
            if (timer >= timeToWait && !firstImageDone) {
                firstImageDone = true;

                // 두 번째 이미지 추가
                if (!secondImageAdded) {
                    AnimatedObject* image2 = new AnimatedObject(
                        "C:/Source/project_pkmbattle/Client/assets/bojeong.png",
                        { 0, 0 }, 200.f, 1.0f
                    );

                    aniManager.add(*image2, [](AnimatedObject& obj2, float dt) {
                        obj2.intro(dt);
                        });

                    secondImageAdded = true;
                }
            }
            });
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {}

    void update(sf::RenderWindow& window) override {
        aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
    }

    void render(sf::RenderWindow& window) override {
        // 배경을 먼저 그린 후, 다른 이미지들 렌더링
        if (background.has_value()) {
            window.draw(*background);
        }
        aniManager.renderAll(window);
    }
};
