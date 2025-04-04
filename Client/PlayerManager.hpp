#pragma once
#include "pch.h"
#include "Player.hpp"
#include "UIChattingBox.hpp"

class PlayerManager {
private:
    Player MyPlayer;
    std::unordered_map<int, Player> otherPlayers;
    UIChattingBox chatting;
    PlayerManager() = default;
    int capHolderId = -1;
    int lostId = -1;

public:
    // 싱글톤 접근자
    static PlayerManager& getInstance() {
        static PlayerManager instance;
        return instance;
    }

    // 복사 금지
    PlayerManager(const PlayerManager&) = delete;
    PlayerManager& operator=(const PlayerManager&) = delete;

    void make_MyPlayer(int id, std::string nickname, int _x, int _y, int _win, int _lose, int _level, int _exp);
    // 루프용 3종세트
    void handleInput(const sf::Event& event, sf::RenderWindow& window);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    // 서버 응답 처리
    void handleEvent(std::string tag, std::string msg);

    // 플레이어 관리
    void addPlayer(int id, std::string nickname, int _x, int _y, int _win, int _lose, int _level, int _exp);
    void removePlayer(int id);

    Player* getPlayer(int id);
    Player& getMyPlayer();
    std::unordered_map<int, Player>& getPlayers();
    UIChattingBox& getChatUI() { return chatting; }

    int getCapHolderId() { return capHolderId; }
    int getLostId() { return lostId; }
    void setLostId(int id) {
        lostId = id;
    }
    void setCapHolderId(int id) {
        capHolderId = id;
    }
};
