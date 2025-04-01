#pragma once
#include "pch.h"
#include "KeyManager.hpp"
#include "BaseScene.hpp"
#include "SceneManager.hpp"

#include "TitleScene.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"


class OpeningScene : public BaseScene {
private:
    sf::Font font;
    sf::Text text;
    AnimationManager aniManager;
    std::unique_ptr<AnimatedObject> MADEBY;
    std::unique_ptr<AnimatedObject> KHSJ;
    
public:
    OpeningScene()
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , text(font,"SKIP : ESC", 24)
    {};

    void init() override {
        text.setPosition({ 650, 0 });
        MADEBY = std::unique_ptr<AnimatedObject>(new AnimatedObject("C:/Source/project_pkmbattle/Client/assets/MADEBY.png",
            { 250 , 100 }, 200.f, 0.f));
            
        KHSJ = std::unique_ptr<AnimatedObject>(new AnimatedObject("C:/Source/project_pkmbattle/Client/assets/KHSJ.png",
            { 250 , 200 }, 200.f, 0.f));

        AnimatedObject Chansoo(
            "C:/Source/project_pkmbattle/Client/assets/chansoo.png",
            { 250, 400 }, 200.f , 0.5f , 0
        );
        AnimatedObject Bojeong(
            "C:/Source/project_pkmbattle/Client/assets/bojeong.png",
            { 300, 400 }, 200.f , 1.0f , 0
        );
        AnimatedObject Dongkwan(
            "C:/Source/project_pkmbattle/Client/assets/dongwan.png",
            { 350, 400 }, 200.f , 1.5f , 0
        );
        AnimatedObject Seokjun(
            "C:/Source/project_pkmbattle/Client/assets/seokjun.png",
            { 400, 400 }, 200.f , 2.0f , 0
        );
        AnimatedObject Hyeongwoo(
            "C:/Source/project_pkmbattle/Client/assets/hyeongwoo.png",
            { 450, 400 }, 200.f , 2.5f , 0
        );
        AnimatedObject Bigjun(
            "C:/Source/project_pkmbattle/Client/assets/bigjun.png",
            { 500, 400 }, 200.f , 3.0f , 0 
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

    void handleEvent(std::string tag, std::string msg) override {

    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Escape)) {
            SceneManager::getInstance().changeScene("title");
        }
    }

    void update(sf::RenderWindow& window) override {
        bool shouldChange = aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
        if (shouldChange) {
            SceneManager::getInstance().changeScene("title");
        }
    }

    void render(sf::RenderWindow& window) override {
        window.draw(text);
        MADEBY->draw(window);
        KHSJ->draw(window);
        aniManager.renderAll(window);
    }
};