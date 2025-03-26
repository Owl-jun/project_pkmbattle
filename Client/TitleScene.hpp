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
    std::optional<sf::Sprite> background;
    UIManager uiManager;
    //Player player;

public:
    TitleScene() 
        : font("C:/Windows/Fonts/arial.ttf")
        , title(font, "WELCOME", 60) 
        , backgroundTex()
    {
    }

    void init() override {
        if (!backgroundTex.loadFromFile("C:/Source/project_pkmbattle/Client/assets/test.jpg")) {
            std::cerr << "배경 이미지 로딩 실패!\n";
        }
        background.emplace(backgroundTex);
        uiManager.addElement(new UIButton(
            { 300,400 },        // 버튼 위치 
            { 200,50 },         // 버튼 크기
            "START",            // 버튼 텍스트
            sf::Color::White,   // 버튼 색
            font,
            []() {
                std::cout << "버튼 클릭됨!" << std::endl;
                SceneManager::getInstance().changeScene(new LoginScene()); } // command
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
        auto offset = KeyManager::getInstance().getDirection();
        uiManager.update(window);

    }


    void render(sf::RenderWindow& window) override {
        window.draw(*background);
        window.draw(title);
        uiManager.render(window);
    }

};
