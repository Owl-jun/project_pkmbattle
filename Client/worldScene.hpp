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
        
        if (isChatting)
        {
            chatBox->render(window);
            chatBox->setPos({ camera.getCenter().x - 300.f , camera.getCenter().y + 200.f });
            chaticon.render(window);
        }
    }
    // ---------------------------------------------------------------------------------

};
