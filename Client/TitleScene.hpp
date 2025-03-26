#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.h"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "Player.h"

class TitleScene : public BaseScene {
private:
    sf::Font font;
    sf::Text title;
    sf::Texture backgroundTex;
    sf::Texture birdTex;
    sf::Texture ksTex;
    sf::Texture hjTex;
    std::optional<sf::Sprite> background;
    std::optional<sf::Sprite> bird;
    std::optional<sf::Sprite> ks;
    std::optional<sf::Sprite> hj;
    sf::Vector2f birdPos;
    sf::Vector2f ksPos;
    sf::Vector2f hjPos;
    float birdBaseY;
    float birdTime;
    float ksY;
    float hjY;
    int birdDir = -1;
    int ksDir = 1;
    int hjDir = -1;

    UIManager uiManager;
    //Player player;

public:
    TitleScene() 
        : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.ttf")
        , title(font, "", 60) 
    {
    }

    void init() override {
        if (!backgroundTex.loadFromFile("C:/Source/project_pkmbattle/Client/assets/introbg.png")) {
            std::cerr << "배경 이미지 로딩 실패!\n";
        }
        if (!ksTex.loadFromFile("C:/Source/project_pkmbattle/Client/assets/kstext.png")) {
            std::cerr << "강 석 텍스트이미지 로딩실패\n";
        }
        if (!hjTex.loadFromFile("C:/Source/project_pkmbattle/Client/assets/hjtext.png")) {
            std::cerr << "황 준 텍스트이미지 로딩실패\n";
        }
        if (!birdTex.loadFromFile("C:/Source/project_pkmbattle/Client/assets/introbird.png")) {
            std::cerr << "새 이미지 로딩 실패!\n";
        }

        background.emplace(backgroundTex);
        sf::Vector2f bgtextureSize = static_cast<sf::Vector2f>(backgroundTex.getSize());
        sf::Vector2f windowSize = static_cast<sf::Vector2f>(GameManager::getInstance().getWindow().getSize());
        background->setScale({ (windowSize.x / bgtextureSize.x), (windowSize.y / bgtextureSize.y) });
        background->setPosition({ 0.f, 0.f });

        birdBaseY = windowSize.y / 2.f - 100.f;
        birdTime = 0.f;
        birdPos = { windowSize.x / 2.f - 100 , birdBaseY };
        sf::Vector2f birdtextureSize = static_cast<sf::Vector2f>(birdTex.getSize());
        bird.emplace(birdTex);
        bird->setOrigin({ bird->getLocalBounds().position.x / 2.f, bird->getLocalBounds().position.x / 2.f });
        bird->setScale({ (windowSize.x / birdtextureSize.x)/3, (windowSize.y / birdtextureSize.y)/3 });
        bird->setPosition(birdPos);

        ksY = -50.f;
        ksPos = { 240 , ksY };
        sf::Vector2f kstextureSize = static_cast<sf::Vector2f>(ksTex.getSize());
        ks.emplace(ksTex);
        ks->setScale({ (windowSize.x / kstextureSize.x)/2.f , (windowSize.y / kstextureSize.y)/3.f });
        ks->setPosition(ksPos);

        hjY = 20.f;
        hjPos = { 240 , hjY };
        sf::Vector2f hjtextureSize = static_cast<sf::Vector2f>(hjTex.getSize());
        hj.emplace(hjTex);
        hj->setScale({ (windowSize.x / hjtextureSize.x)/2.f , (windowSize.y / hjtextureSize.y)/3.f });
        hj->setPosition(hjPos);

        uiManager.addElement(new UIButton(
            { 300,400 },        // 버튼 위치 
            { 200,50 },         // 버튼 크기
            "START",            // 버튼 텍스트
            sf::Color(0,0,0,0),   // 버튼 색
            font,
            []() { SceneManager::getInstance().changeScene(new LoginScene()); } // command
        ));

        title.setFillColor(sf::Color::Black);
        title.setPosition({ 200, 250 });
    }

    void update(sf::RenderWindow& window) override {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            KeyManager::getInstance().handleEvent(*event);
            uiManager.handleEvent(*event,window);
        }
        uiManager.update(window);

        float speed = 800.f;   // 좌우 이동 속도 (픽셀/초)
        float amplitude = 120.f; // 위아래 진폭
        float freq = 4.f;       // 위아래 움직임 속도 (Hz)

        // 시간 갱신
        static sf::Clock clock;
        float dt = clock.restart().asSeconds();

        sf::Vector2f birdtextureSize = static_cast<sf::Vector2f>(birdTex.getSize());
        sf::Vector2f windowSize = static_cast<sf::Vector2f>(GameManager::getInstance().getWindow().getSize());
        birdTime += dt;
        if (birdPos.x <= -300) {
            birdPos.x = windowSize.x + 100;
        }
        birdPos.x += speed * dt * birdDir;
        birdPos.y = birdBaseY + std::sin(birdTime * freq) * amplitude;
        bird->setPosition(birdPos);


        if (ksPos.y <= -50) {
            ksDir = 1;
        }
        else if (ksPos.y >= 0) {
            ksDir = -1;
        }
        ksPos.y += 100.f * dt * ksDir;
        ks->setPosition(ksPos);

        if (hjPos.y <= -50) {
            hjDir = 1;
        }
        else if (hjPos.y >= 0) {
            hjDir = -1;
        }
        hjPos.y += 100.f * dt * hjDir;
        hj->setPosition(hjPos);
    }


    void render(sf::RenderWindow& window) override {
        window.draw(*background);
        window.draw(*ks);
        window.draw(*hj);
        window.draw(*bird);
        uiManager.render(window);
    }

};
