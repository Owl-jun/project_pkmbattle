#pragma once
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "SceneManager.hpp"
#include "LoginScene.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "UITextBox.hpp"
#include "TimeManager.hpp"
#include "ResourceManager.hpp"
#include "Player.h"
#include "SettingsOverlay.hpp"
#include "NetworkManager.hpp"
#include "GameManager.h"
#include "SelectOverlay.hpp"
#include "CharacterSelectOverlay.hpp"


class worldScene : public BaseScene {
private:
    sf::Font font;
    sf::Text frame;
    std::string deltatime;
    sf::Texture bgTex;
    std::optional<sf::Sprite> bg;
    UIManager uiManager;

    // TextBox
    UITextBox* chatBox;
    bool isChatting = false;

    Player& player;
    std::unordered_map<int, Player> otherPlayers;
    int myId;

    sf::View camera;
    SettingsOverlay settings;
    
    bool waitingForOpponent = false;
    SelectOverlay* overlay = nullptr;   // 추가댐

    CharacterSelectOverlay* charSelector = nullptr;     // 추가

    float escCooldown = 0.f;
    float overlayCloseTimer = -1.f; // 기본값: 작동 안함


public:
    worldScene()
        : font(ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
        , deltatime("0")
        , frame(font, deltatime, 24)
        , player(SceneManager::getInstance().getPlayer())
        , settings({ 800.f,600.f }, font)
        , myId(-1)
        , chatBox(new UITextBox({ 100.f,500.f }, { 600.f,40.f }, font))
    {
        chatBox->setFocus(false);
        myId = NetworkManager::getInstance().getMyId();
        std::cout << "my id : " << myId << std::endl;

        overlay = new SelectOverlay({ 400.f, 200.f }, font);
        overlay->setCenter({ 400.f, 300.f });


        // 🔹 싸운다 누르면 캐릭터 선택창 띄우기
        overlay->setFightCallback([this]() {
            charSelector->show();
            overlay->hide();
            });

        // 🔹 캐릭터 선택 완료 시 배틀씬 진입
        charSelector = new CharacterSelectOverlay({ 800.f, 600.f }, font, [this](const std::vector<int>& selected) {
            std::cout << "[선택된 포켓몬] ";
            for (int idx : selected) std::cout << idx << " ";
            std::cout << std::endl;

            // 👉 카메라 위치 수동 고정
            overlay->setCenter({ 400.f, 300.f }); // ending 씬에서 보여줄 위치로 맞춰줘

            // 👉 카메라 플레이어 따라다니는 기능 끄고 싶으면 flag 써도 됨
            // cameraFollowPlayer = false;

            // 배틀씬 진입
            SceneManager::getInstance().changeScene("ending");
            });
    }

    ~worldScene() {
        delete overlay;
    }        // 추가 (동관)

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
        if (isChatting) {
            chatBox->handleEvent(event, window);

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

        // 상호작용
        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Space) && escCooldown <= 0.f) {
            sf::Vector2i frontTile = player.getTileInFront();
            for (auto& [id, p] : otherPlayers) {
                if (p.getTilePosition() == frontTile) {
                    std::string toSend = "INTERACT " + std::to_string(id) + "\n";
                    NetworkManager::getInstance().send(toSend);
                    escCooldown = 0.5f;
                    break;
                }
            }
        }

        // 환경설정창
        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Escape) && escCooldown <= 0.f) {
            settings.toggle();
            std::cout << "Visible: " << settings.isVisible() << "\n";
            escCooldown = 0.5f;
        }

        // 🔹 1키 누르면 SelectOverlay 토글 <- 이거 기능구현 후 없애야함. (부딪혔거나, 특정 위치값에 갔을경우)
        if (KeyManager::getInstance().isKeyDown(sf::Keyboard::Key::Num1) && escCooldown <= 0.f) {
            overlay->toggle();
            escCooldown = 0.5f;
        }

        overlay->handleEvent(event, window); // 🔹 overlay 이벤트 전달
        charSelector->handleEvent(event, window);
        settings.handleEvent(event, window);
    }

    void update(sf::RenderWindow& window) override {
        float dt = TimeManager::getInstance().getDeltaTime();
        escCooldown -= dt;
        if (overlayCloseTimer > 0.f) {
            overlayCloseTimer -= dt;
            if (overlayCloseTimer <= 0.f) {
                std::cout << "[Timer] overlay 자동 종료!\n";  // 추가
                overlay->hide();
                overlay->setWaiting(false); // 상태 초기화
                sf::String logm = L"누군가 싸움을 걸었다!";
                overlay->setMessage(logm); // 기본 메시지 복원
            }
        }
        int fps = static_cast<int>(1.f / dt);
        frame.setString("FPS: " + std::to_string(fps));

        // 서버 응답 처리
        std::string response = NetworkManager::getInstance().receive();
        if (!response.empty()) {
            std::istringstream iss(response);
            std::string type;
            iss >> type;

            if (type == "PLAYERS")
            {
                int id, x, y;
                std::string d;
                while (iss >> id >> x >> y >> d)
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
                        sf::Vector2i otherTile = { x , y };
                        if (it != otherPlayers.end())
                        {
                            // 이미 존재하면 위치만 갱신
                            it->second.setTargetTilePosition({ x, y });
                            it->second.setCurDir(d);
                        }
                        else
                        {
                            // 처음 등장한 플레이어
                            Player newPlayer(otherTile.x, otherTile.y);
                            newPlayer.setTargetTilePosition({ x, y });
                            otherPlayers[id] = newPlayer;
                        }
                    }
                }
                // 접속 종료한 플레이어 제거
                std::unordered_set<int> activeIds;
                iss.clear(); iss.seekg(0); std::string dummy; iss >> dummy; // 다시 읽기 위해 rewind
                while (iss >> id >> x >> y >> d) activeIds.insert(id);

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
            else if (type == "CHAT") {
                int senderId;
                std::string id , msg;
                iss >> senderId >> id;
                iss >> std::ws;
                std::getline(iss,msg);
                

                std::cout << "[Chat] From " << id << ": " << msg << std::endl;

                if (senderId == myId) {
                    player.showSpeechBubble(msg, font);
                }
                else if (otherPlayers.count(senderId)) {
                    otherPlayers[senderId].showSpeechBubble(msg, font);
                }
                
            }
            else if (type == "INTERACTION") {
                int id1, id2;
                iss >> id1 >> id2;

                if (myId == id1 || myId == id2) {
                    // 본인 혹은 대상인 경우 상호작용 UI 띄우기
                    showInteractionUI(id1, id2);
                }
                else {
                    // 관전자라면 알림 UI 띄우기
                    showOtherPlayersInteraction(id1, id2);
                }
            }
            else if (type == "FIGHT_RESULT") {
                int id1, id2;
                std::string status;
                iss >> id1 >> id2 >> status;

                if (status == "CANCELED") {
                    waitingForOpponent = false;
                    overlay->disableCallbacks();  // ✨ 직접 구현 필요
                    std::wstring logm = L"상대방이 도망쳤습니다!";
                    overlay->setMessage(logm);
                    overlay->setWaiting(true);
                    overlayCloseTimer = 1.f;

                    // 선택 비활성화 (둘 다 종료 상태로 처리)
                }


                else if (status == "READY") {
                    std::wcout << L"싸움이 시작됩니다!" << std::endl;
                    waitingForOpponent = false;
                    overlay->hide(); // UI 닫기
                    charSelector->show(); // 전투 전 캐릭터 선택 UI로
                }
                
            }

            else {
                std::cout << "[Client] Unknown server response: " << response << "\n";
            }


        }

        if (settings.isVisible() || overlay->isVisible() || charSelector->isVisible()) {
        }
        else {
            player.update(dt, true);   // 아무것도 없으면 움직임 가능
        }
        for (auto& [id, p] : otherPlayers) {
            p.update(dt, false);
        }

        camera.setCenter(player.getPosition());
        chatBox->setPos({ camera.getCenter().x -300.f , camera.getCenter().y + 200.f});
        chatBox->update(window);
        settings.setCenter(camera.getCenter());
        overlay->setCenter(camera.getCenter());

        settings.update(window);
        overlay->update(window); // 🔹 overlay 업데이트
        charSelector->update(window); // 🔹 charSelector 업데이트
        window.setView(camera);

        // frame을 카메라 기준 화면 좌상단에 배치
        sf::Vector2f topLeft = camera.getCenter() - camera.getSize() / 2.f;
        frame.setPosition(topLeft + sf::Vector2f(10.f, 10.f));
        

    }

    void render(sf::RenderWindow& window) override {
        // 카메라 뷰에서 맵/캐릭터 렌더링
        window.setView(camera);

        if (bg.has_value()) window.draw(*bg);

        player.draw(window);

        for (auto& [id, p] : otherPlayers) {
            p.draw(window);
        }

        player.draw(window);

        if (isChatting)
            chatBox->render(window);

        window.draw(frame);  
        settings.render(window);
        // 동관이
        overlay->render(window); // 🔹 overlay 렌더링
        charSelector->render(window); 
    }
    
    void showInteractionUI(int id1, int id2) {
        overlay->toggle();
        overlay->setFightCallback([this]() {
            if (overlay->isVisible() && !waitingForOpponent) {
                std::string msg = "FIGHT_RESPONSE " + std::to_string(myId) + " FIGHT\n";
                NetworkManager::getInstance().send(msg);
                overlay->setMessage(L"상대방의 선택을 기다리는 중...");
                overlay->setWaiting(true); // 버튼 숨기고 메시지만
                waitingForOpponent = true;
            }
        });

        overlay->setRunCallback([this]() {
            if (overlay->isVisible() && !waitingForOpponent) {
                std::string msg = "FIGHT_RESPONSE " + std::to_string(myId) + " RUN\n";
                NetworkManager::getInstance().send(msg);

                overlay->setMessage(L"도망치는 중...");   // 또는 원하는 메시지
                overlay->setWaiting(true);                // 버튼 숨기고 메시지 띄움
                overlayCloseTimer = 2.f;                  // 2초 후 자동 닫힘
            }
        });


    }

    void showOtherPlayersInteraction(int id1, int id2) {
        std::cout << "👀 ID " << id1 << "과(와) ID " << id2 << "가 뭔가 하고 있음..." << std::endl;
        // 캐릭터 근처에 말풍선 표시도 가능
    }



};
