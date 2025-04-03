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

class worldScene : public BaseScene {
private:
    sf::Font font;
    UIManager uiManager;
    AnimationManager aniManager;
    float keyCooldown = 0.f;

    std::unordered_map<int, Player>& otherPlayers = PlayerManager::getInstance().getPlayers();
    Player& myPlayer = PlayerManager::getInstance().getMyPlayer();
    sf::Texture capTex = ResourceManager::getInstance().getTextureByName("capC.png");
    sf::Sprite cap = sf::Sprite(capTex);
    bool capView = true;
    
    // 채팅 UI
    UITextBox* chatBox;
    bool isChatting = false;


    UIChatIcon chaticon;

    // CAM
    sf::View camera;


public:
    worldScene()
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , chatBox(new UITextBox({ 100.f,500.f }, { 600.f,40.f }, 24))
        , chaticon({0.f,0.f}, {60.f,30.f},24)
    {
        cap.setPosition({ 6 * 60.f,5 * 60.f });
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
        PlayerManager::getInstance().handleEvent(tag, line);
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
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
                        std::cout << "CHCOLOR 송신완료" << std::endl;
                    }
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

        PlayerManager::getInstance().getChatUI().handleInput(event,window);
        PlayerManager::getInstance().handleInput(event,window);
    }

    void update(sf::RenderWindow& window) override {
        float dt = TimeManager::getInstance().getDeltaTime();
        keyCooldown -= dt;
        PlayerManager::getInstance().update(dt);
        camera.setCenter(PlayerManager::getInstance().getMyPlayer().getPosition());

        PlayerManager::getInstance().getChatUI().update(window);
        PlayerManager::getInstance().getChatUI().setPos({ camera.getCenter().x - 400.f , camera.getCenter().y + 60.f});
        chaticon.setPos({ 
            PlayerManager::getInstance().getMyPlayer().getPosition().x
            , PlayerManager::getInstance().getMyPlayer().getPosition().y - 55.f
            });
        chaticon.update(window);
        chatBox->update(window);
        window.setView(camera);     

    }

    void render(sf::RenderWindow& window) override {
        // 카메라 뷰에서 맵/캐릭터 렌더링
        window.setView(camera);
        aniManager.renderAll(window);
        PlayerManager::getInstance().draw(window);
        PlayerManager::getInstance().getChatUI().render(window);

        if (PlayerManager::getInstance().getCapHolderId() == -1) {
            // 누구도 안가졌을 때
            myPlayer.setGetCap(false);
            for (auto& [id, p] : otherPlayers) {
                p.setGetCap(false);
            }
            window.draw(cap);
        }
        else if (PlayerManager::getInstance().getCapHolderId() == NetworkManager::getInstance().getSocketID()) {
            // 내가 소유자일 때
            myPlayer.setGetCap(true);
        }
        else {
            // 다른 사람이 가졌을 때
            for (auto& [id, p] : otherPlayers) {
                if (id == PlayerManager::getInstance().getCapHolderId()) {
                    p.setGetCap(true);
                }
            }
        }

        if (isChatting)
        {
            chatBox->render(window);
            chatBox->setPos({ camera.getCenter().x - 300.f , camera.getCenter().y + 200.f });
            // chaticon.render(window);
        }
    }
    // ---------------------------------------------------------------------------------

};
