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
            "C:/Source/project_pkmbattle/Client/assets/chansoo.png",
            { 250, 300 }, 200.f , 0.5f
        );
        AnimatedObject Bojeong(
            "C:/Source/project_pkmbattle/Client/assets/bojeong.png",
            { 300, 300 }, 200.f , 1.0f
        );
        AnimatedObject Dongkwan(
            "C:/Source/project_pkmbattle/Client/assets/dongwan.png",
            { 350, 300 }, 200.f , 1.5f
        );
        AnimatedObject Seokjun(
            "C:/Source/project_pkmbattle/Client/assets/seokjun.png",
            { 400, 300 }, 200.f , 2.0f
        );
        AnimatedObject Hyeongwoo(
            "C:/Source/project_pkmbattle/Client/assets/hyeongwoo.png",
            { 450, 300 }, 200.f , 2.5f
        );
        AnimatedObject Bigjun(
            "C:/Source/project_pkmbattle/Client/assets/bigjun.png",
            { 500, 300 }, 200.f , 3.0f
        );

        aniManager.add(Chansoo, [](AnimatedObject& obj, float dt)
            {
                obj.intro(dt);
            }
        );
        aniManager.add(Bojeong, [](AnimatedObject& obj, float dt)
            {
                obj.intro(dt);
            }
        );
        aniManager.add(Dongkwan, [](AnimatedObject& obj, float dt)
            {
                obj.intro(dt);
            }
        );
        aniManager.add(Seokjun, [](AnimatedObject& obj, float dt)
            {
                obj.intro(dt);
            }
        );
        aniManager.add(Hyeongwoo, [](AnimatedObject& obj, float dt)
            {
                obj.intro(dt);
            }
        );
        aniManager.add(Bigjun, [](AnimatedObject& obj, float dt)
            {
                obj.intro(dt);
            }
        );
        
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Escape)) {
            SceneManager::getInstance().changeScene(new TitleScene());
        }
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