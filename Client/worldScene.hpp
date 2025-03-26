#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.h"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "Player.h"

class worldScene : public BaseScene {
private:
    sf::Font font;
    sf::Text title;
    UIManager uiManager;

public:
    worldScene() 
        : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF")
        , title(font, "WORLD SCENE", 60) 
    {}

    void init() override {
        title.setFillColor(sf::Color::White);
        title.setPosition({ 200, 250 });
        
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {

    }

    void update(sf::RenderWindow& window) override {

    }

    void render(sf::RenderWindow& window) override {
        window.draw(title);
    }
};
