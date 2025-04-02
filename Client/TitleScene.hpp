#pragma once
#include "pch.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"

#include "UIButton.hpp"
#include "UIManager.hpp"

#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"

class TitleScene : public BaseScene {
private:
    AnimationManager aniManager;
    UIManager uiManager;
    
    sf::Font font;
    
    sf::Vector2f bgtextureSize;
    sf::Vector2f windowSize;

public:
    TitleScene() 
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
    {
    }

    void init() override {
        auto bg = AnimatedObject("introbg.png", { 0.f,0.f });
        bgtextureSize = static_cast<sf::Vector2f>(ResourceManager::getInstance().getTextureByName("introbg.png").getSize());
        windowSize = static_cast<sf::Vector2f>(GameManager::getInstance().getWindow().getSize());
        bg.setScale({ (windowSize.x / bgtextureSize.x), (windowSize.y / bgtextureSize.y) });
        aniManager.add(bg);
        
        AnimatedObject bird("introbird.png", {windowSize.x/2, windowSize.y/2}, 850.f, 2.f, -1.f, 50.f);
        bird.setScale({ 0.25,0.25 });
        bird.setOriginCenter();

        AnimatedObject text1("kstext.png", { windowSize.x/2.f, 36 } , 100.f, 0.f, 1.f, 0.f);
        AnimatedObject text2("hjtext.png", { windowSize.x / 2.f, 89 },100.f, 0.f, -1.f, 0.f);
        text1.setScale({ 0.2,0.2 });
        text2.setScale({ 0.2,0.2 });
        text1.setOriginCenter();
        text2.setOriginCenter();

        aniManager.add(bird, [](AnimatedObject& obj, float dt) { obj.move(dt, 800.f);} );
        aniManager.add(text1, [](AnimatedObject& obj, float dt) { obj.bounce(dt, 35, 90); } );
        aniManager.add(text2, [](AnimatedObject& obj, float dt) { obj.bounce(dt, 35, 90); } );

        uiManager.addElement(new UIButton({ 300,400 },{ 200,50 },"START",48,sf::Color(0,0,0,0), []() { SceneManager::getInstance().changeScene("login"); } ));
    }

    // 서버 응답처리 함수
    void handleEvent(std::string tag, std::string line) {}
    
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        uiManager.handleInput(event, window);
    }

    void update(sf::RenderWindow& window) override {
        aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
        uiManager.update(window);
    }

    void render(sf::RenderWindow& window) override {
        aniManager.renderAll(window);
        uiManager.render(window);
    }

};
