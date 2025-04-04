#pragma once
#include "pch.h"
#include "KeyManager.hpp"
#include "SceneManager.hpp"
#include "UIManager.hpp"
#include "AnimationManager.hpp"
#include "TimeManager.hpp"
#include "PlayerManager.hpp"

#include "BaseScene.hpp"
#include "UITextBox.hpp"
#include "UiChatIcon.hpp"

#include "SettingsOverlay.hpp"

//#define TIME 20.f;

class worldScene : public BaseScene {
private:
    sf::Font font = ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF");
    SettingsOverlay settings = SettingsOverlay({ 800.f,600.f }, font);
    UIManager uiManager;
    AnimationManager aniManager;
    float keyCooldown = 0.f;

    std::unordered_map<int, Player>& otherPlayers = PlayerManager::getInstance().getPlayers();
    Player& myPlayer = PlayerManager::getInstance().getMyPlayer();
    sf::Texture capTex = ResourceManager::getInstance().getTextureByName("capC.png");
    sf::Sprite desk = sf::Sprite(ResourceManager::getInstance().getTextureByName("desk.png"));
    sf::Sprite teacher = sf::Sprite(ResourceManager::getInstance().getTextureByName("teachernormal.png"));
    sf::Sprite cap = sf::Sprite(capTex);
    bool capView = true;
    
    // 채팅 UI
    UITextBox* chatBox;
    bool isChatting = false;

    UIChatIcon chaticon;

    bool isControl = true;
    // CAM
    sf::View camera;
    
    // 모자돌리기 게임
    sf::Sprite gameTitle = sf::Sprite(ResourceManager::getInstance().getTextureByName("gametitle.png"));
    bool gameOn = false;
    float gameTimer = 20.f;
    sf::Text gameTimerText = sf::Text(font, std::wstring(L""), 60);

public:
    worldScene()
        : chatBox(new UITextBox({ 100.f,500.f }, { 600.f,40.f }, 24))
        , chaticon({0.f,0.f}, {60.f,30.f},24)
    {
        teacher.setPosition({ 6 * 60.f , 5 * 60.f + 30 });
        desk.setPosition({ 4 * 60.f, 5 * 60.f +  5.f });
        cap.setPosition({ 6 * 60.f + 4.f,5 * 60.f +10.f});
        cap.setScale({ 0.9f,0.9f });
        chatBox->setFocus(false);
    }

    void init() override {
        AnimatedObject bg("worldMap.png", { 60.f,60.f });
        aniManager.add(bg);

        // 카메라 설정
        camera.setSize({ 800.f, 600.f });
        camera.setCenter(PlayerManager::getInstance().getMyPlayer().getPosition());
    }


    // ----------------------------------------------------------------------------------
    // 4종세트
    void handleEvent(std::string tag, std::string line) {
        if (tag == "GAMESTATE") {
            std::istringstream iss(line);
            std::string state;
            float time;
            iss >> state;

            if (state == "ON") {
                iss >> time;
                gameOn = true;
                gameTimer = time;
                gameTimerText.setFillColor(sf::Color::White); // 초기화
            }
        }   
        EventManager::getInstance().clearEvents(tag);
        PlayerManager::getInstance().handleEvent(tag, line);
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        if (event.is<sf::Event::KeyPressed>()) {
            auto key = event.getIf<sf::Event::KeyPressed>();
            if (key && key->code == sf::Keyboard::Key::Escape) {
                settings.toggle();
            }
        }
        if (settings.isVisible()) {
            settings.handleInput(event,window);
            isControl = false;
        }
        else { isControl = true; }

        if (isChatting) {
            chatBox->handleInput(event, window);

            if (event.is<sf::Event::KeyPressed>()) {
                auto key = event.getIf<sf::Event::KeyPressed>();
                if (key && key->code == sf::Keyboard::Key::Enter) {
                    std::string msg = chatBox->getInput();
                    if (!msg.empty()) {
                        std::string toSend = "CHAT " + msg + "\n";
                        NetworkManager::getInstance().send(toSend);
                    }
                    chatBox->clear();
                    isChatting = false;
                }
            }
            return;
        }

        // 채팅창이 비활성일 때 Enter를 누르면 채팅모드 진입
        if (event.is<sf::Event::KeyPressed>()) {
            auto key = event.getIf<sf::Event::KeyPressed>();
            if (key && key->code == sf::Keyboard::Key::Enter) {
                isChatting = true;
                chatBox->setFocus(true);
                return;
            }
            if (key && key->code == sf::Keyboard::Key::Space) {
                int x = 16;
                for (int i = 0; i < 4; ++i) {
                    if (PlayerManager::getInstance().getMyPlayer().getTileInFront() == sf::Vector2i(x + i, 8))
                    {
                        std::string toSend = "CHCOLOR " + std::to_string(i) + "\n";
                        NetworkManager::getInstance().send(toSend);
                        //std::cout << "CHCOLOR 송신완료" << std::endl;
                    }
                }

                if (PlayerManager::getInstance().getMyPlayer().getTileInFront() == sf::Vector2i(27, 3)) {
                    std::string toSend = "SAFE\n";
                    NetworkManager::getInstance().send(toSend);
                }

                if (PlayerManager::getInstance().getCapHolderId() != -1) {
                    std::string Send = "GETCAP SEND "
                        + std::to_string(PlayerManager::getInstance().getMyPlayer().getTileInFront().x) + " "
                        + std::to_string(PlayerManager::getInstance().getMyPlayer().getTileInFront().y) + "\n";
                    NetworkManager::getInstance().send(Send);
                }
                if (PlayerManager::getInstance().getMyPlayer().getTileInFront() == sf::Vector2i(6, 5)) {
                    if (PlayerManager::getInstance().getCapHolderId() == -1) {
                        std::string toSend = "GETCAP GET\n";
                        NetworkManager::getInstance().send(toSend);
                    }
                    else if (PlayerManager::getInstance().getCapHolderId() == NetworkManager::getInstance().getSocketID()) {
                        std::string toSend = "GETCAP LOST\n";
                        NetworkManager::getInstance().send(toSend);
                    }
                }
            }
        }

        if (isControl) {
            PlayerManager::getInstance().getChatUI().handleInput(event,window);
            PlayerManager::getInstance().handleInput(event,window);
        }
    }

    void update(sf::RenderWindow& window) override {
        for (const std::string& tag : { "GAMESTATE" }) {
            auto events = EventManager::getInstance().getEvents(tag);
            for (const auto& msg : events) {
                handleEvent(tag, msg);
            }
        }
        float dt = TimeManager::getInstance().getDeltaTime();
        keyCooldown -= dt;
        PlayerManager::getInstance().update(dt);
        camera.setCenter(PlayerManager::getInstance().getMyPlayer().getPosition());

        
        settings.setCenter(camera.getCenter());
        settings.update(window);
        

        gameTitle.setPosition({ camera.getCenter().x - 215.f , camera.getCenter().y - 290.f });
        gameTimerText.setPosition({ camera.getCenter().x - 70.f , camera.getCenter().y - 170.f });

        PlayerManager::getInstance().getChatUI().update(window);
        PlayerManager::getInstance().getChatUI().setPos({ camera.getCenter().x - 400.f , camera.getCenter().y + 60.f});
        chaticon.setPos({ 
            PlayerManager::getInstance().getMyPlayer().getPosition().x
            , PlayerManager::getInstance().getMyPlayer().getPosition().y - 55.f
            });
        chaticon.update(window);
        chatBox->update(window);
        window.setView(camera);     
        
        // 모자 관련 로직
        if (PlayerManager::getInstance().getCapHolderId() != -1) {
            teacher.setTexture(ResourceManager::getInstance().getTextureByName("teacherangry.png"));
            gameOn = true;
        }
        else
        {
            teacher.setTexture(ResourceManager::getInstance().getTextureByName("teachernormal.png"));
            gameOn = false;
        }
    }

    void render(sf::RenderWindow& window) override {
        // 카메라 뷰에서 맵/캐릭터 렌더링
        window.setView(camera);
        aniManager.renderAll(window);
        window.draw(teacher);
        window.draw(desk);
        PlayerManager::getInstance().draw(window);
        PlayerManager::getInstance().getChatUI().render(window);

        // 모자돌리기 시작...
        if (gameOn) {
            window.draw(gameTitle);
            gameTimer -= TimeManager::getInstance().getDeltaTime();
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << gameTimer;
            gameTimerText.setString(sf::String(oss.str()));
            window.draw(gameTimerText);

            if (gameTimer <= 10.f) { gameTimerText.setFillColor(sf::Color::Red); }
            else { gameTimerText.setFillColor(sf::Color::White); }

            // 게임 끝 로직
            if (gameTimer <= 0.f) { 
                int loser = PlayerManager::getInstance().getCapHolderId();
                std::string toSend = "LOSER " + std::to_string(loser) + "\n";
                NetworkManager::getInstance().send(toSend);
                
                PlayerManager::getInstance().setLostId(loser);
                PlayerManager::getInstance().setCapHolderId(-1);
                gameOn = false; 
            }
        }
        

        // 모자관련 로직
        if (PlayerManager::getInstance().getCapHolderId() == -1) {
            myPlayer.setGetCap(false);
            for (auto& [id, p] : otherPlayers) {    
                p.setGetCap(false);
                
            }
            window.draw(cap);
        }
        else if (PlayerManager::getInstance().getCapHolderId() == NetworkManager::getInstance().getSocketID()) {
            // 내가 소유자일 때
            myPlayer.setGetCap(true);
            for (auto& [id, p] : otherPlayers) {
                p.setGetCap(false);    
            }
        }
        else {
            // 다른 사람이 가졌을 때
            myPlayer.setGetCap(false);
            for (auto& [id, p] : otherPlayers) {
                if (id == PlayerManager::getInstance().getCapHolderId()) {
                    p.setGetCap(true);
                }
                if (id == PlayerManager::getInstance().getLostId()) {
                    p.setGetCap(false);
                }
            }
        }
        // 

        
        settings.render(window);
        

        if (isChatting)
        {
            chatBox->setPos({ camera.getCenter().x - 300.f , camera.getCenter().y + 200.f });
            chatBox->render(window);
            // chaticon.render(window);
        }
    }


    float roundToDecimalPlaces(float value, int places) {
        float factor = std::pow(10.0f, places);
        return std::round(value * factor) / factor;
    }
    // ---------------------------------------------------------------------------------

};
