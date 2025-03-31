#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"
#include "ResourceManager.hpp"
#include "GameManager.h"

class TitleScene : public BaseScene {
private:
    sf::Font font;
    sf::Texture backgroundTex;
    std::optional<sf::Sprite> background;
    sf::Vector2f bgtextureSize;
    sf::Vector2f windowSize;
    AnimationManager aniManager;
    UIManager uiManager;

public:
    TitleScene() 
        : font(ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
    {
    }

    void init() override {
        backgroundTex = ResourceManager::getInstance().getTexture("C:/Source/project_pkmbattle/Client/assets/introbg.png");
        bgtextureSize = static_cast<sf::Vector2f>(backgroundTex.getSize());
        windowSize = static_cast<sf::Vector2f>(GameManager::getInstance().getWindow().getSize());
        background.emplace(backgroundTex);
        background->setScale({ (windowSize.x / bgtextureSize.x), (windowSize.y / bgtextureSize.y) });
        background->setPosition({ 0.f, 0.f });
        
        AnimatedObject bird("C:/Source/project_pkmbattle/Client/assets/introbird.png",
            {windowSize.x/2, windowSize.y/2}, 
            850.f,
            0.f,
            255.f,
            50.f,
            7.f
        );
        bird.setScale({ 0.25,0.25 });
        bird.setOriginCenter();

        AnimatedObject text1("C:/Source/project_pkmbattle/Client/assets/kstext.png",
            { windowSize.x/2.f, 36 },
            100.f, 0.f, 255.f, 0.f,0.f,1
        );

        AnimatedObject text2("C:/Source/project_pkmbattle/Client/assets/hjtext.png",
            { windowSize.x / 2.f, 89 },
            100.f, 0.f, 255.f,0.f,0.f, -1
        );

        text1.setScale({ 0.2,0.2 });
        text2.setScale({ 0.2,0.2 });
        text1.setOriginCenter();
        text2.setOriginCenter();

        aniManager.add(bird, [](AnimatedObject& obj, float dt) 
            {
                obj.move(dt, 800.f);
            }
        );
        aniManager.add(text1, [](AnimatedObject& obj, float dt)
            {
                obj.bounce(dt, 35, 90);
            }
        );
        aniManager.add(text2, [](AnimatedObject& obj, float dt)
            {
                obj.bounce(dt, 35, 90);
            }
        );

        uiManager.addElement(new UIButton(
            { 300,400 },        // 버튼 위치 
            { 200,50 },         // 버튼 크기
            "START",            // 버튼 텍스트
            sf::Color(0,0,0,0),   // 버튼 색
            font,
            []() { SceneManager::getInstance().changeScene("login"); } // command
        ));

    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        uiManager.handleEvent(event, window);
    }

    void update(sf::RenderWindow& window) override {
        aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
        uiManager.update(window);
    }


    void render(sf::RenderWindow& window) override {
        window.draw(*background);
        aniManager.renderAll(window);
        uiManager.render(window);
    }

};
