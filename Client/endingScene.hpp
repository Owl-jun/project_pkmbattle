#pragma once

// ------------------------------------------
// 씬 템플릿
// ------------------------------------------

#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
#include "TitleScene.hpp"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"

//#include <SFML/Graphics.hpp>

class endingScene : public BaseScene {
private:
    sf::Font font;
    sf::View camera;// 250401 카메라 셋팅World -> ending
    std::vector<sf::Text> credits;
    float scrollSpeed = 30.0f; // 픽셀/초
    float yOffset = 600.f;     // 시작 위치
    float spacing = 50.f;      // 줄 사이 간격

    AnimationManager aniManager;

public:
    endingScene()
        : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSK.TTF") {
    }
    void init() override {
        // 💡 카메라 초기화 // 250401 카메라 셋팅World -> ending
        camera.setSize({ 800.f, 600.f });
        camera.setCenter({ 400.f, 300.f });  // 중앙 기준 위치
        std::vector<std::wstring> lines = {
            L"Thanks for playing !",
            L"PuKyung IoT Mini Project ",
            L"Project 피까츄게임 엔진 만들기",
            L"팀명 : 강 황 석 준",
            L"팀 장",
            L"강석준",
            L" ",   // 여백줄때는 SPACE 한칸 뛰우삼(폰트 높이 보정때문)
            L"팀    원",
            L"최 보 정",
            L"황 석 준",
            L"이 형 우",
            L"김 찬 수",
            L"우 동 관",
            L" ",
            L"제작기간",
            L"13일",
            L" ",
            L"Special Thanks",
            L"중국집 사장님",
            L"컵라면",
            L"팀장님(출석체크맨날 빼먹음)",
            L"",
            L"SFML (폰트 진짜 열받넹ㅋ)",
            L" ",
            L"BUG Issue",
            L"이게 왜 안돼?",
            L"이게 왜 돼?",
            L"버그가 아니라 기능임.",
            L"다음 버전은 안나옵니다.",
            L"우리가 찾은건 3개",
            L"님들이 찾은건 100개일지도",
            L"캐릭터 픽셀은 직접 그렸습니다.",
            L"(대충 그렸습니다.)",
            L"THE END",
            L"Thank You!"
        };


        std::vector<unsigned int> sizes = {     // 행길이별로 사이즈크기 조정하는거임
    36, 28, 28, 33, 50, 45, 10, 40, 30, 30,     // 40 ~ 50행
    30, 30, 30, 30, 38, 30, 20, 36, 22, 22,
    22, 22, 22, 20, 40, 30, 30, 22, 22, 22,
    22, 22, 22, 22, 35, 40
        };

        float currentY = yOffset;

        for (size_t i = 0; i < lines.size(); ++i) {
            unsigned int size = (i < sizes.size()) ? sizes[i] : 24;

            sf::Text text(font, lines[i], size);
            text.setFillColor(sf::Color::White);

            // 텍스트 가로 길이를 가져와서 가운데 정렬
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y });
            text.setPosition({ 800.f / 2.f, currentY });

            currentY += spacing;
            credits.push_back(text);

        }

        // 첫 번째 애니메이션: HoCulman
        AnimatedObject hoculman(
            "C:/Source/project_pkmbattle/Client/assets/ending1.png",
            sf::Vector2f(400.f, 500.f),
            20.f
        );
        hoculman.setScale({ 2.f, 2.f });

        aniManager.add(hoculman, [this](AnimatedObject& obj, float dt) {
            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });

        // 두 번째 애니메이션: CreditsDoll (예시)
        AnimatedObject doll(
            "C:/Source/project_pkmbattle/Client/assets/ending2.png",
            sf::Vector2f(50.f, 700.f),  // 등장 위치 조절 가능
            20.f
        );
        doll.setScale({ 2.f, 2.f });

        aniManager.add(doll, [this](AnimatedObject& obj, float dt) {
            static float delay = 3.0f; // HoCulman이 먼저 나오게 약간의 지연
            if (delay > 0.f) {
                delay -= dt;
                return;
            }

            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });
        // 세 번째 애니메이션
        AnimatedObject doll2(
            "C:/Source/project_pkmbattle/Client/assets/ending3.png",
            sf::Vector2f(450.f, 700.f),  // 등장 위치 조절 가능
            20.f
        );
        doll2.setScale({ 2.f, 2.f });

        aniManager.add(doll2, [this](AnimatedObject& obj, float dt) {
            static float delay = 10.0f;
            if (delay > 0.f) {
                delay -= dt;
                return;
            }

            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });

        // 네번째 이미지
        AnimatedObject doll3(
            "C:/Source/project_pkmbattle/Client/assets/ending4.png",
            sf::Vector2f(50.f, 700.f),
            20.f
        );
        doll3.setScale({ 2.f, 2.f });
        aniManager.add(doll3, [this](AnimatedObject& obj, float dt) {
            static float delay = 18.0f;
            if (delay > 0.f) {
                delay -= dt;
                return;
            }
            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });

        AnimatedObject doll4(
            "C:/Source/project_pkmbattle/Client/assets/ending5.png",
            sf::Vector2f(450.f, 700.f),
            20.f
        );
        doll4.setScale({ 2.f, 2.f });
        aniManager.add(doll4, [this](AnimatedObject& obj, float dt) {
            static float delay = 26.0f;
            if (delay > 0.f) {
                delay -= dt;
                return;}
            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });


        AnimatedObject doll5(
            "C:/Source/project_pkmbattle/Client/assets/PiGon3.png",
            sf::Vector2f(50.f, 700.f),
            20.f
        );
        doll5.setScale({ 1.f, 1.f });
        aniManager.add(doll5, [this](AnimatedObject& obj, float dt) {
            static float delay = 32.0f;
            if (delay > 0.f) {
                delay -= dt;
                return;
            }
            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });

        AnimatedObject doll6(
            "C:/Source/project_pkmbattle/Client/assets/PiGon.png",
            sf::Vector2f(450.f, 700.f),
            20.f
        );
        doll6.setScale({ 1.f, 1.f });
        aniManager.add(doll6, [this](AnimatedObject& obj, float dt) {
            static float delay = 38.0f;
            if (delay > 0.f) {
                delay -= dt;
                return;
            }
            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });

        AnimatedObject doll7(
            "C:/Source/project_pkmbattle/Client/assets/PiGon2.png",
            sf::Vector2f(50.f, 700.f),
            20.f
        );
        doll7.setScale({ 1.f, 1.f });

        aniManager.add(doll7, [this](AnimatedObject& obj, float dt) {
            static float delay = 42.0f;
            if (delay > 0.f) {
                delay -= dt;
                return;
            }

            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });

        AnimatedObject doll8(
            "C:/Source/project_pkmbattle/Client/assets/PiGon3.png",
            sf::Vector2f(450.f, 700.f),
            20.f
        );
        doll8.setScale({ 1.f, 1.f });
        aniManager.add(doll8, [this](AnimatedObject& obj, float dt) {
            static float delay = 42.0f;
            if (delay > 0.f) {
                delay -= dt;
                return;
            }

            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Escape)) {
            SceneManager::getInstance().changeScene("opening");
        }
    }

    void update(sf::RenderWindow& window) override {
        float delta = TimeManager::getInstance().getDeltaTime();
        
        for (auto& text : credits) {
            text.move({ 0.f, -scrollSpeed * delta });
        }

        aniManager.updateAll(delta);

    }

    void render(sf::RenderWindow& window) override {
        window.setView(camera);     // 250401 카메라 셋팅World -> ending
        window.clear(sf::Color::Black); // 흑백 배경 (검정)
        for (auto& text : credits) {
            window.draw(text);
        }

        aniManager.renderAll(window);


    }
};
