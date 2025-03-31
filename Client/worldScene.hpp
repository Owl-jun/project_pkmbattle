#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "TimeManager.hpp"
#include "ResourceManager.hpp"
#include "Player.h"
#include "SettingsOverlay.hpp"
#include "NetworkManager.hpp"
#include "SelectOverlay.hpp"
#include "GameManager.h"

class worldScene : public BaseScene {
private:
    sf::Font font;
    sf::Text frame;
    std::string deltatime;
    sf::Texture bgTex;
    std::optional<sf::Sprite> bg;
    UIManager uiManager;

    Player& player;
    std::unordered_map<int, Player> otherPlayers;
    int myId;

    sf::View camera;
    SettingsOverlay settings;
    SelectOverlay selectOverlay;
    float escCooldown = 0.f;
    float selectCooldown = 0.f;

public:
    worldScene()
        : font(ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
        , deltatime("0")
        , frame(font, deltatime, 24)
        , player(GameManager::getInstance().getPlayer())
        , settings({ 800.f,600.f }, ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
        , selectOverlay({ 400.f, 200.f }, ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
        , myId(-1)
    {
        myId = NetworkManager::getInstance().getMyId();
        std::cout << "my id : " << myId << std::endl;
    }

    void init() override {
        frame.setFont(font);
        frame.setPosition({ 0.f,10.f });
        frame.setFillColor(sf::Color::White);
        bgTex = ResourceManager::getInstance().getTexture("C:/Source/project_pkmbattle/Client/assets/worldMap.png");
        bg.emplace(bgTex);
        bg->setPosition({ 60.f,60.f });

        // 카메라 설정
        camera.setSize({ 800.f, 600.f });
        camera.setCenter(player.getPosition());
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Escape) && escCooldown <= 0.f) {
            settings.toggle();
            std::cout << "Visible: " << settings.isVisible() << "\n";
            escCooldown = 0.5f;
        }

        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Num1) && selectCooldown <= 0.f) {
            selectOverlay.toggle();
            selectOverlay.setCenter(camera.getCenter());
            std::cout << "SelectOverlay Visible: " << selectOverlay.isVisible() << "\n";
            selectCooldown = 0.5f;
        }

        if (selectOverlay.isVisible()) {
            selectOverlay.handleEvent(event, window);
        }
        else {
            settings.handleEvent(event, window);
        }
    }


    void update(sf::RenderWindow& window) override {
        float dt = TimeManager::getInstance().getDeltaTime();
        escCooldown -= dt;
        selectCooldown -= dt;
        int fps = static_cast<int>(1.f / dt);
        frame.setString("FPS: " + std::to_string(fps));

        // 서버 응답 받아서 위치 반영
        std::string response = NetworkManager::getInstance().receive();

        if (!response.empty()) {
            std::istringstream iss(response);
            std::string type;
            iss >> type;

            if (type == "PLAYERS")
            {
                int id, x, y;
                while (iss >> id >> x >> y)
                {
                    if (id == myId)
                    {
                        sf::Vector2i serverTile = { x, y };
                        if (serverTile != player.getTilePosition()) {
                            player.setTargetTilePosition(serverTile); // 변경된 경우에만!
                        }
                    }
                    else
                    {
                        auto it = otherPlayers.find(id);
                        if (it != otherPlayers.end())
                        {
                            // 이미 존재하면 위치만 갱신
                            it->second.setTargetTilePosition({ x, y });
                        }
                        else
                        {
                            // 처음 등장한 플레이어
                            Player newPlayer;
                            newPlayer.setTargetTilePosition({ x, y });
                            otherPlayers[id] = newPlayer;
                        }
                    }
                }
                // 접속 종료한 플레이어 제거
                std::unordered_set<int> activeIds;
                iss.clear(); iss.seekg(0); std::string dummy; iss >> dummy; // 다시 읽기 위해 rewind
                while (iss >> id >> x >> y) activeIds.insert(id);

                for (auto it = otherPlayers.begin(); it != otherPlayers.end(); )
                {
                    if (activeIds.find(it->first) == activeIds.end()) {
                        it = otherPlayers.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }

            else {
                std::cout << "[Client] Unknown server response: " << response << "\n";
            }
        }

        if (!settings.isVisible() && !selectOverlay.isVisible()) {
            player.update(dt, true);  // 설정창 열리면 멈춤
        }
        for (auto& [id, p] : otherPlayers) {
            p.update(dt, false);
        }
        camera.setCenter(player.getPosition());
        settings.setCenter(camera.getCenter());
        settings.update(window);
        window.setView(camera);

        // frame을 카메라 기준 화면 좌상단에 배치
        sf::Vector2f topLeft = camera.getCenter() - camera.getSize() / 2.f;
        frame.setPosition(topLeft + sf::Vector2f(10.f, 10.f));

        if (selectOverlay.isVisible()) {
            selectOverlay.update(window);
        }
    }

    void render(sf::RenderWindow& window) override {
        // 카메라 뷰에서 맵/캐릭터 렌더링
        window.setView(camera);

        if (bg.has_value()) window.draw(*bg);

        player.draw(window);

        for (auto& [id, p] : otherPlayers) {
            p.draw(window);
        }

        window.draw(frame);

        settings.render(window);

        if (selectOverlay.isVisible()) {
            selectOverlay.render(window);
        }
    }
};