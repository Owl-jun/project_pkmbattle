#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "TitleScene.hpp"
#include "SceneManager.h"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"

class OpeningScene : public BaseScene {
private:
    AnimationManager aniManager;
    
public:
    OpeningScene() {};

    void init() override {
        AnimatedObject Chansoo(
            "C:/Source/project_pkmbattle/Client/assets/logo1.png",
            { 200, 200 }, 200.f
        );
        aniManager.add(std::move(Chansoo), [](AnimatedObject& obj, float dt)
            {
                obj.intro(dt);
            }
        );
       
    }
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {

    }

    void update(sf::RenderWindow& window) override {
        bool shouldChange = aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
        if (shouldChange) {
            SceneManager::getInstance().changeScene(new TitleScene());
        }
    }

    void render(sf::RenderWindow& window) override {
        aniManager.renderAll(window);
    }
};