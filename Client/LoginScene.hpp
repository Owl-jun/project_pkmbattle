#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.h"
#include "LoginScene.hpp"
#include "worldScene.hpp"
#include "UIButton.hpp"
#include "UITextBox.hpp"
#include "UIManager.hpp"
#include "Player.h"

class LoginScene : public BaseScene {
private:
    sf::Font font;
    sf::Text title;
    UIManager uiManager;
    UITextBox* idBox = nullptr;
    UITextBox* pwBox = nullptr;

public:
    LoginScene() : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"), title(font, "LOGIN SCENE", 60) {} 

    void init() override {
        title.setFillColor(sf::Color::White);
        title.setPosition({ 200, 50 });
        idBox = new UITextBox(
            { 300.f, (272.f + 28.f) },
            { 200.f, 28.f },
            font
        );
        pwBox = new UITextBox(
            { 300.f, (272.f + 56.f) },
            { 200.f, 28.f },
            font
        );
        uiManager.addElement(idBox);
        uiManager.addElement(pwBox);
        
        uiManager.addElement(new UIButton(
            { 500.f, 300.f },
            { 150.f, 56.f },
            "LOGIN",
            sf::Color::White,
            font,
            [this]() { 
                std::string id = idBox->getInput();
                std::string pw = pwBox->getInput();
                id.erase(remove(id.begin(), id.end(),' '),id.end());
                pw.erase(remove(pw.begin(), pw.end(),' '),pw.end());
                std::cout << "[LOGIN] ID: " << id << ", PW: " << pw << "\n";

                if (id == "admin" && pw == "1234") {
                    SceneManager::getInstance().changeScene(new worldScene());
                }
                else {
                    std::cerr << "로그인 실패!\n";
                }
            }
        ));
    }

    void update(sf::RenderWindow& window) override {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            KeyManager::getInstance().handleEvent(*event);
            uiManager.handleEvent(*event, window);
        }
        uiManager.update(window);
    }

    void render(sf::RenderWindow& window) override {
        window.draw(title);
        uiManager.render(window);
    }
};
