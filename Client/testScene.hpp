#pragma once

// ------------------------------------------
// 씬 템플릿
// ------------------------------------------

#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
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

public:
    testScene()
        : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.ttf")
    {
    }

    void init() override {
        UIButton testButton(
            { 1.f,2.f },
            { 100,200 },
            "버튼임ㅋ",
            sf::Color::Black,
            font,
            []() {
                std::cout << "버튼눌림" << std::endl;
            }
        );

        AnimatedObject testImage(
            "C:/Source/project_pkmbattle/Client/fonts/logo.png"
            , { 10.f, 20.f }
        );

        uiManager.addElement(&testButton);
        aniManager.add(testImage);
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        //if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::A)) {
        //    
        //}
    }

    void update(sf::RenderWindow& window) override {
        uiManager.update(window);
        aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
    }


    void render(sf::RenderWindow& window) override {
        uiManager.render(window);
        aniManager.renderAll(window);
    }

};
