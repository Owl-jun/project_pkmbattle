#pragma once
#include "pch.h"
#include "SceneManager.hpp"
#include "KeyManager.hpp"
#include "BaseScene.hpp"
#include "TimeManager.hpp"
#include "PlayerManager.hpp"
#include "GameManager.hpp"

#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"

#include "UIButton.hpp"
#include "UITextBox.hpp"
#include "UIManager.hpp"

#include "Player.hpp"

class LoginScene : public BaseScene {
private:
    AnimationManager aniManager;
    UIManager uiManager;

    AnimatedObject bg;
    sf::Vector2f bgtextureSize;
    sf::Vector2f windowSize;

    sf::Font font;

    sf::Text id;
    sf::Text pswd;
    sf::RectangleShape textbox;
    sf::Text warning;
    sf::Text warning2;

    UITextBox* idBox = nullptr;
    UITextBox* pwBox = nullptr;

    bool warningVisible = false;
    int warningInt = 1;
    float timer = 0.f;

public:
    LoginScene()
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , id(font, "ID", 30)
        , pswd(font, "PASSWORD", 30)
        , warning(font, L"이미 접속중인 아이디입니다! 다시시도 : ESC", 24)
        , warning2(font, L"아이디 혹은 비밀번호가 다릅니다! 다시시도 : ESC", 24)
    {
    }

    void init() override {
        bg = AnimatedObject("introbg.png", { 0.f,0.f });
        bgtextureSize = static_cast<sf::Vector2f>(ResourceManager::getInstance().getTextureByName("introbg.png").getSize());
        windowSize = static_cast<sf::Vector2f>(GameManager::getInstance().getWindow().getSize());
        bg.setScale({ (windowSize.x / bgtextureSize.x), (windowSize.y / bgtextureSize.y) });
        bg.setPosition({ 0.f, 0.f });
        aniManager.add(bg, [](AnimatedObject& obj, float dt) {});

        id.setFillColor(sf::Color::Black);
        id.setPosition({ 270, 277.f });
        pswd.setFillColor(sf::Color::Black);
        pswd.setPosition({ 180, 305.f });

        textbox.setFillColor({ 0, 0, 0, 255 });
        textbox.setSize({ 400.f,100.f });
        textbox.setPosition({ (windowSize.x / 2.f) - 200.f , (windowSize.y / 2.f) - 50.f });
        warning.setPosition({ textbox.getPosition().x + 50.f, textbox.getPosition().y + 20.f });
        warning2.setPosition({ textbox.getPosition().x + 30.f, textbox.getPosition().y + 20.f });

        idBox = new UITextBox( { 300.f, (272.f + 28.f) }, { 200.f, 28.f }, 24 );
        pwBox = new UITextBox( { 300.f, (272.f + 56.f) }, { 200.f, 28.f }, 24 );
        idBox->setFocus(true);

        uiManager.addElement(idBox);
        uiManager.addElement(pwBox);       
        uiManager.addElement(new UIButton(
            { 505.f, 300.f },
            { 150.f, 56.f },
            "LOGIN", 48,
            sf::Color::Color(0,0,0,20),
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
                std::cout << "[LoginScene] 로그인 요청 전송완료 "<< "\n";
            }
        ));
        
        AnimatedObject Seokjun("seokjun.png", { 700, 100 }, 20.f, 0.f, -1);
        Seokjun.setScale({ 2.f,2.f });
        aniManager.add(Seokjun, [](AnimatedObject& obj, float dt) { obj.move(dt); });

        AnimatedObject gugu("gugu.png", { 600, 100 }, 50.f, 0.f, -1);
        gugu.setScale({ 1.1f,1.1f });
        aniManager.add(gugu, [](AnimatedObject& obj, float dt) { obj.move(dt, 800.f); });
    }

    // 서버 응답 처리로직
    void handleEvent(std::string tag, std::string line) {
        std::istringstream iss(line);
        std::string response;
        iss >> response;
        std::cout << "[LoginScene] handleEvent : 수신 tag = " << tag << " response : " << response << std::endl;

        if (tag == "LOGIN") {
            if (response == "TRUE") {
                std::vector<std::string> tokens;
                std::string token;

                // 남은 데이터 토큰화
                while (iss >> token) {
                    tokens.push_back(token);
                }

                size_t playerDataSize = 8; // id, nickname, x, y, win, lose, level, exp
                size_t playerCount = (tokens.size() / playerDataSize);

                for (size_t i = 0; i < playerCount; ++i) {
                    size_t offset = i * playerDataSize;

                    int id = std::stoi(tokens[offset + 0]);
                    std::string nickname = tokens[offset + 1];
                    int x = std::stoi(tokens[offset + 2]);
                    int y = std::stoi(tokens[offset + 3]);
                    int win = std::stoi(tokens[offset + 4]);
                    int lose = std::stoi(tokens[offset + 5]);
                    int level = std::stoi(tokens[offset + 6]);
                    float exp = std::stof(tokens[offset + 7]);

                    if (i == 0) {
                        PlayerManager::getInstance().make_MyPlayer(id, nickname, x, y, win, lose, level, exp);
                    }
                    else {
                        PlayerManager::getInstance().addPlayer(id, nickname, x, y, win, lose, level, exp);
                    }
                }

                SceneManager::getInstance().changeScene("world");
            }
            
            else if (response == "EXIST") {
                std::cout << "이미 로그인중인 유저!\n";
                warningVisible = true;
                timer = 2.f;
                warningInt = 1;
            }
            else {
                std::cout << "로그인 실패!\n";
                warningVisible = true;
                timer = 2.f;
                warningInt = 2;
            }
            EventManager::getInstance().clearEvents(tag);
        }
        else {
            std::cout << "잘못된 tag 형식입니다." << std::endl;
        }
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Escape))
        {
            warningVisible = false;
        }
        uiManager.handleInput(event, window);
    }

    void update(sf::RenderWindow& window) override {
        // 수신 패킷이름 지정
        for (const std::string& tag : { "LOGIN" }) {
            auto events = EventManager::getInstance().getEvents(tag);
            for (const auto& msg : events) {
                handleEvent(tag, msg);
            }
        }
        timer -= TimeManager::getInstance().getDeltaTime();
        if (timer <= 0.f) { warningVisible = false; }
        aniManager.updateAll(TimeManager::getInstance().getDeltaTime());
        uiManager.update(window);
    }

    void render(sf::RenderWindow& window) override {
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

};
