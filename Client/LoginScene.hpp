#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
#include "LoginScene.hpp"
#include "worldScene.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"
#include "UIButton.hpp"
#include "UITextBox.hpp"
#include "UIManager.hpp"
#include "Player.h"

class LoginScene : public BaseScene {
private:
    sf::Texture bgTex;
    std::optional<sf::Sprite> bg;
    sf::Vector2f bgtextureSize;
    sf::Vector2f windowSize;
    sf::Font font;
    sf::Text id;
    sf::Text pswd;
    AnimationManager aniManager;
    UIManager uiManager;
    UITextBox* idBox = nullptr;
    UITextBox* pwBox = nullptr;

public:
    LoginScene() 
        : font("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF")
        , id(font, "ID", 30) 
        , pswd(font,"PASSWORD",30) 
    {}

    void init() override {
        bgTex.loadFromFile("C:/Source/project_pkmbattle/Client/assets/introbg.png");
        bgtextureSize = static_cast<sf::Vector2f>(bgTex.getSize());
        windowSize = static_cast<sf::Vector2f>(GameManager::getInstance().getWindow().getSize());
        bg.emplace(bgTex);
        bg->setScale({ (windowSize.x / bgtextureSize.x), (windowSize.y / bgtextureSize.y) });
        bg->setPosition({ 0.f, 0.f });

        id.setFillColor(sf::Color::Black);
        pswd.setFillColor(sf::Color::Black);
        id.setPosition({ 270, 277.f });
        pswd.setPosition({ 180, 305.f });


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
            { 505.f, 300.f },
            { 150.f, 56.f },
            "LOGIN",
            sf::Color::White,
            font,
            [this]() { 
                std::string id = idBox->getInput();
                std::string pw = pwBox->getInput();
                id.erase(remove(id.begin(), id.end(),' '),id.end());
                id.erase(remove(id.begin(), id.end(),'\t'),id.end());
                id.erase(remove(id.begin(), id.end(),'\n'),id.end());
                pw.erase(remove(pw.begin(), pw.end(),' '),pw.end());
                pw.erase(remove(pw.begin(), pw.end(),'\t'),pw.end());
                pw.erase(remove(pw.begin(), pw.end(),'\n'),pw.end());
                std::cout << "[LOGIN] ID: " << id << ", PW: " << pw << "\n";

                if (id == "admin" && pw == "1234") {
                    SceneManager::getInstance().changeScene(new worldScene());
                }
                else {
                    std::cerr << "로그인 실패!\n";
                }
            }
        ));
        
        AnimatedObject Seokjun(
            "C:/Source/project_pkmbattle/Client/assets/seokjun.png",
            { 700, 100 }, 20.f, 0.f, 255.f , 1.f , 2.f , -1
        );
        Seokjun.setScale({ 2.f,2.f });
        aniManager.add(Seokjun, [](AnimatedObject& obj, float dt) {
            obj.move(dt);
            });

        AnimatedObject gugu(
            "C:/Source/project_pkmbattle/Client/assets/gugu.png",
            { 600, 100 }, 50.f, 0.f, 255.f, 100.f, 2.f, -1
        );
        gugu.setScale({ 1.1f,1.1f });
        aniManager.add(gugu, [](AnimatedObject& obj, float dt) {
            obj.move(dt, 800.f);
        });

    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        uiManager.handleEvent(event, window);
    }

    void update(sf::RenderWindow& window) override {
        aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
        uiManager.update(window);
    }

    void render(sf::RenderWindow& window) override {
        window.draw(*bg);
        aniManager.renderAll(window);
        window.draw(id);
        window.draw(pswd);
        uiManager.render(window);
    }
};
