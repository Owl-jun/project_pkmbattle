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

// 테스트임
#include "UIChatIcon.hpp"

class worldScene : public BaseScene {
private:
    sf::Font font;
    UIManager uiManager;
    AnimationManager aniManager;
    float keyCooldown = 0.f;
    

    // 채팅 UI 보정
    UITextBox* chatBox;
    bool isChatting = false;
    // CAM 보정
    sf::View camera;
    // 말풍선 보정
    UIChatIcon* chatIcon = nullptr;
    bool isChatBubbleActive = false;


public:
    worldScene()
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , chatBox(new UITextBox({ 100.f,500.f }, { 600.f,40.f }, 24))
    {
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
                    chatBox->setFocus(false);
                }
            }
            return;
        }

        // 채팅모드 진입
        if (event.is<sf::Event::KeyPressed>()) {
            auto key = event.getIf<sf::Event::KeyPressed>();

            if (key && key->code == sf::Keyboard::Key::Enter) {
                isChatting = true;
                chatBox->setFocus(true);
                return;
            }
        
         // 말풍선 생성 (1번 키) 보정
        if (key && key->code == sf::Keyboard::Key::Num1) {
            if (!isChatBubbleActive) {
                sf::Vector2f playerPos = PlayerManager::getInstance().getMyPlayer().getPosition();
                sf::Vector2f iconSize = { 100.f, 40.f };
                sf::Vector2f iconPos = playerPos + sf::Vector2f(-iconSize.x / 2.f, -80.f);

                chatIcon = new UIChatIcon(iconPos, iconSize, 20);
                isChatBubbleActive = true;
            }
            return;
        }
        }
            PlayerManager::getInstance().handleInput(event, window);
    }


    void update(sf::RenderWindow& window) override {
        float dt = TimeManager::getInstance().getDeltaTime();
        keyCooldown -= dt;
        
        PlayerManager::getInstance().update(dt);

        camera.setCenter(PlayerManager::getInstance().getMyPlayer().getPosition());
        chatBox->setPos({ camera.getCenter().x -300.f , camera.getCenter().y + 200.f});
        PlayerManager::getInstance().getChatUI().setPos({ camera.getCenter().x - 400.f , camera.getCenter().y + 60.f});
        chatBox->update(window);
        window.setView(camera);     

        // 채팅창 위치 업데이트 보정
        chatBox->setPos({ camera.getCenter().x - 300.f , camera.getCenter().y + 180.f });
        chatBox->update(window);
    
        // 💬 말풍선 위치 & 애니메이션 갱신 보정
        if (isChatBubbleActive && chatIcon) {
            sf::Vector2f playerPos = PlayerManager::getInstance().getMyPlayer().getPosition();
            sf::Vector2f iconSize = { 100.f, 40.f };
            sf::Vector2f iconPos = playerPos + sf::Vector2f(-iconSize.x / 2.f, -80.f);
            chatIcon->setPos(iconPos);
            chatIcon->update(window);
        }

    }

    void render(sf::RenderWindow& window) override {
        // 카메라 뷰에서 맵/캐릭터 렌더링
        window.setView(camera);
        aniManager.renderAll(window);
        PlayerManager::getInstance().draw(window);
        PlayerManager::getInstance().getChatUI().render(window);
        if (isChatting)
            chatBox->render(window);

        // 말풍선 렌더링 보정
        if (isChatBubbleActive && chatIcon)
            chatIcon->render(window);
    }
    // ---------------------------------------------------------------------------------

};
