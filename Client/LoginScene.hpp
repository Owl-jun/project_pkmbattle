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
#include "ResourceManager.hpp"
#include "NetworkManager.hpp"


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
        : font(ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
        , id(font, "ID", 30)
        , pswd(font, "PASSWORD", 30)
    {
    }

    void init() override {
        // 이미지 생성 예시 , 리소스매니저에서 받고 , emplace 
        bgTex = ResourceManager::getInstance().getTexture("C:/Source/project_pkmbattle/Client/assets/introbg.png");
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
        idBox->setFocus(true);
        uiManager.addElement(idBox);
        uiManager.addElement(pwBox);
        
        // 로그인 로직
        uiManager.addElement(new UIButton(
            { 505.f, 300.f },
            { 150.f, 56.f },
            "LOGIN",
            sf::Color::Color(0,0,0,20),
            font,
            [this]() {
                std::string id = idBox->getInput();
                std::string pw = pwBox->getInput();

                auto clean = [](const std::string& str) {
                    std::string result;
                    for (char c : str) {
                        if (!isspace(static_cast<unsigned char>(c)))
                            result += c;
                    }
                    return result;
                    };

                id = clean(id);
                pw = clean(pw);

                std::cout << "[LOGIN] ID: " << id << ", PW: " << pw << "\n";

                std::string msg = "LOGIN " + id + " " + pw + "\n";
                NetworkManager::getInstance().send(msg);

                std::string tag, response , x , y;
                std::string line = NetworkManager::getInstance().receive_line();
                std::cout << line << std::endl;
                std::istringstream iss(line);
                iss >> tag >> response >> x >> y;
                std::cout << tag << response << x << y << std::endl;
                
                if (tag == "LOGIN") {
                    std::cout << "로그인 태그 받음" << std::endl;
                    if (response == "TRUE") {
                        std::cout << "login complete!\n";            
                        sf::Vector2f pos = { std::stoi(x)*60.f, std::stoi(y) * 60.f };
                        std::cout << pos.x << pos.y << std::endl;
                        GameManager::getInstance().getPlayer().setPosition(pos);
                        std::cout << "setTile 실행" << std::endl;
                        SceneManager::getInstance().changeScene("world");
                    }
                    else {
                        std::cout << "login failed!\n";
                        return;
                    }
                }
                else { return; }
            }
        ));

        AnimatedObject Seokjun(
            "C:/Source/project_pkmbattle/Client/assets/seokjun.png",
            { 700, 100 }, 20.f, 0.f, 255.f, 1.f, 2.f, -1
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
        // 서버연결없이 테스트용
        if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::F5)) {
            SceneManager::getInstance().changeScene("world");
        }
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
