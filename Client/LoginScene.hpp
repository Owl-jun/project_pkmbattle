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
#include "GameManager.h"

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

    bool warningVisible = false;
    int warningInt = 1;
    sf::RectangleShape textbox;
    sf::Text warning;
    sf::Text warning2;

    bool loginCompleted = false;
    std::thread receiveThread;


public:
    LoginScene()
        : font(ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
        , id(font, "ID", 30)
        , pswd(font, "PASSWORD", 30)
        , warning(font, L"이미 접속중인 아이디입니다! 다시시도 : ESC", 24)
        , warning2(font, L"아이디 혹은 비밀번호가 다릅니다! 다시시도 : ESC", 24)
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

        textbox.setFillColor({ 0, 0, 0, 255 });
        textbox.setSize({ 400.f,100.f });
        textbox.setPosition({ (windowSize.x / 2.f) - 200.f , (windowSize.y / 2.f) - 50.f });
        warning.setPosition({ textbox.getPosition().x + 50.f, textbox.getPosition().y + 20.f });
        warning2.setPosition({ textbox.getPosition().x + 30.f, textbox.getPosition().y + 20.f });


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
                        if (!isspace(static_cast<unsigned char>(c)) && c != 27)
                            result += c;
                    }
                    return result;
                    };

                id = clean(id);
                pw = clean(pw);

                std::cout << "[LOGIN] ID: " << id << ", PW: " << pw << "\n";

                std::string msg = "LOGIN " + id + " " + pw + "\n";
                NetworkManager::getInstance().send(msg);

                receiveThread = std::thread([this]() {
                    while (!loginCompleted) {
                        std::string line = NetworkManager::getInstance().receive_block();
                        if (!line.empty() && line.starts_with("LOGIN")) {
                            handleLoginResponse(line);
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    });
                receiveThread.detach();
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
        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Escape))
        {
            warningVisible = false;
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
        if (warningVisible) {
            window.draw(textbox);
            if (warningInt == 1)
                window.draw(warning);
            else if (warningInt == 2)
                window.draw(warning2);
        }
    }

    void handleLoginResponse(const std::string& line) {
        std::istringstream iss(line);
        std::string tag, response, x, y;
        iss >> tag >> response >> x >> y;

        if (tag == "LOGIN") {
            if (response == "TRUE") {
                std::cout << "로그인 성공!\n";
                loginCompleted = true;
                SceneManager::getInstance().make_Player(stoi(x), stoi(y));
                SceneManager::getInstance().registerScene("world", new worldScene());
                SceneManager::getInstance().changeScene("world");
            }
            else if (response == "alreadyLoginPlayer") {
                std::cout << "이미 로그인중인 유저!\n";
                warningVisible = true;
                warningInt = 1;
            }
            else {
                std::cout << "로그인 실패!\n";
                warningVisible = true;
                warningInt = 2;
            }
        }
    }

};
