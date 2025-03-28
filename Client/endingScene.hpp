#pragma once

// ------------------------------------------
// �� ���ø�
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
    std::vector<sf::Text> credits;
    float scrollSpeed = 30.0f; // �ȼ�/��
    float yOffset = 600.f;     // ���� ��ġ
    float spacing = 50.f;      // �� ���� ����

    AnimationManager aniManager;

public:
    endingScene()
        : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSK.TTF") {
    }
    void init() override {
        std::vector<std::wstring> lines = {
            L"Thanks for playing !",
            L"PuKyung IoT Mini Project ",
            L"Project �Ǳ������ ���� �����",
            L"���� : �� Ȳ �� ��",
            L"�� ��",
            L"������",
            L" ",   // �����ٶ��� SPACE ��ĭ �ٿ��(��Ʈ ���� ��������)
            L"��    ��",
            L"�� �� ��",
            L"Ȳ �� ��",
            L"�� �� ��",
            L"�� �� ��",
            L"�� �� ��",
            L" ",
            L"���۱Ⱓ",
            L"13��(����7����������)",
            L" ",
            L"Special Thanks",
            L"Umm Jun Sick",
            L"�Ŷ��",
            L"«?",
            L"����(�߰��׽���)",
            L"SFML (��Ʈ ��¥ ���޳ߤ�)",
            L" ",
            L"BUG Issue",
            L"�̰� �� �ȵ�?",
            L"�̰� �� ��?",
            L"���װ� �ƴ϶� �����.",
            L"���� ������ �ȳ��ɴϴ�.",
            L"�츮�� ã���� 3��",
            L"�Ե��� ã���� 100��������",
            L"���� �� ���� ��� �ȼ��� ���� �׷Ƚ��ϴ�",
            L"(���� �׷Ƚ��ϴ�.)",
            L"THE END",
            L"Thank You!"
        };

        std::vector<unsigned int> sizes = {     // ����̺��� ������ũ�� �����ϴ°���
    36, 28, 28, 33, 50, 45, 10, 40, 30, 30,     // 40 ~ 50��
    30, 30, 30, 30, 38, 30, 20, 36, 22, 22,
    22, 22, 22, 20, 40, 30, 30, 22, 22, 22,
    22, 22, 22, 22, 35, 40
        };

        float currentY = yOffset;

        for (size_t i = 0; i < lines.size(); ++i) {
            unsigned int size = (i < sizes.size()) ? sizes[i] : 24;

            sf::Text text(font, lines[i], size);
            text.setFillColor(sf::Color::White);

            // �ؽ�Ʈ ���� ���̸� �����ͼ� ��� ����
            sf::FloatRect textBounds = text.getLocalBounds();
            float centerX = (800.f - textBounds.size.x) / 2.f; // 800�� ȭ�� ���� �ʺ�
            text.setPosition({ centerX, currentY });

            currentY += spacing;
            credits.push_back(text);

        }

        // ù ��° �ִϸ��̼�: HoCulman
        AnimatedObject hoculman(
            "C:/Source/project_pkmbattle/Client/assets/HoCulman.png",
            sf::Vector2f(300.f, 500.f),
            20.f
        );
        hoculman.setScale({ 2.f, 2.f });

        aniManager.add(hoculman, [this](AnimatedObject& obj, float dt) {
            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });

        // �� ��° �ִϸ��̼�: CreditsDoll (����)
        AnimatedObject doll(
            "C:/Source/project_pkmbattle/Client/assets/PiGon.png",
            sf::Vector2f(50.f, 700.f),  // ���� ��ġ ���� ����
            20.f
        );
        doll.setScale({ 2.f, 2.f });

        aniManager.add(doll, [this](AnimatedObject& obj, float dt) {
            static float delay = 4.0f; // HoCulman�� ���� ������ �ణ�� ����
            if (delay > 0.f) {
                delay -= dt;
                return;
            }

            obj.fadein(dt);
            auto pos = obj.getSprite().getPosition();
            pos.y -= scrollSpeed * dt;
            obj.setPosition(pos);
            });
        // �� ��° �ִϸ��̼�
        AnimatedObject doll2(
            "C:/Source/project_pkmbattle/Client/assets/PiGon2.png",
            sf::Vector2f(330.f, 700.f),  // ���� ��ġ ���� ����
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

        // �׹�° �̹���
        AnimatedObject doll3(
            "C:/Source/project_pkmbattle/Client/assets/PiGon3.png",
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
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Escape)) {
            SceneManager::getInstance().changeScene(new TitleScene());
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
        window.clear(sf::Color::Black); // ��� ��� (����)
        for (auto& text : credits) {
            window.draw(text);
        }

        aniManager.renderAll(window);


    }
};
