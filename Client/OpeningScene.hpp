#pragma once
#include "pch.h"
#include "KeyManager.hpp"
#include "BaseScene.hpp"
#include "SceneManager.hpp"

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

    // 렌더링 객체 생성 및 설정
    void init() override {
        text.setPosition({ 650, 0 });   // SKIP : ESC

        MADEBY = std::unique_ptr<AnimatedObject>(new AnimatedObject("MADEBY.png", { 250 , 100 }));
        KHSJ = std::unique_ptr<AnimatedObject>(new AnimatedObject("KHSJ.png", { 250 , 200 }));

        AnimatedObject Chansoo("chansoo.png", { 250, 400 }, 0.5f, 0.f);
        AnimatedObject Bojeong("bojeong.png", { 300, 400 }, 1.0f, 0.f);
        AnimatedObject Dongkwan("dongwan.png",{ 350, 400 }, 1.5f, 0.f);
        AnimatedObject Seokjun("seokjun.png",{ 400, 400 }, 2.0f, 0.f);
        AnimatedObject Hyeongwoo("hyeongwoo.png",{ 450, 400 },2.5f, 0.f);
        AnimatedObject Bigjun("bigjun.png",{ 500, 400 },3.0f, 0.f);

        aniManager.add(Chansoo, [](AnimatedObject& obj, float dt) { obj.fadein(dt); } );
        aniManager.add(Bojeong, [](AnimatedObject& obj, float dt) { obj.fadein(dt); } );
        aniManager.add(Dongkwan, [](AnimatedObject& obj, float dt) { obj.fadein(dt); } );
        aniManager.add(Seokjun, [](AnimatedObject& obj, float dt) { obj.fadein(dt); } );
        aniManager.add(Hyeongwoo, [](AnimatedObject& obj, float dt) { obj.fadein(dt); } );
        aniManager.add(Bigjun, [](AnimatedObject& obj, float dt) { obj.fadein(dt); } );

    }

    // 서버 이벤트 핸들링
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