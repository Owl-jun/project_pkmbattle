#include "pch.h"
#include "PlayerManager.hpp"
#include "Player.hpp"


void PlayerManager::addPlayer(int id, std::string nickname, int _x, int _y, int _win, int _lose, int _level, int _exp) {
    if (otherPlayers.contains(id)) return;
    Player p(nickname, _x, _y, _win, _lose, _level, _exp);
    otherPlayers[id] = p;
    std::cout << "[PlayerManager] : addPlayer ���� �Ϸ�!" << std::endl;
}

void PlayerManager::make_MyPlayer(int id, std::string nickname, int _x , int _y, int _win , int _lose, int _level, int _exp) 
{

    MyPlayer = Player(nickname, _x, _y, _win, _lose, _level, _exp);
    std::cout << "[PlayerManager] : make_MyPlayer ���� �Ϸ�! id : " << std::to_string(id) << std::endl;
}

// --------------------------------------
// 3����Ʈ
void PlayerManager::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    MyPlayer.handleInput(event, window, true);
}

void PlayerManager::update(float dt) {
    // ���� ��Ŷ�̸� ����
    for (const std::string& tag : { "MOVE" , "NEWUSER" , "EXITUSER", "CHAT"}) {
        auto events = EventManager::getInstance().getEvents(tag);
        for (const auto& msg : events) {
            handleEvent(tag, msg);
        }
    }
    for (auto& [id, player] : otherPlayers) {
        player.update(dt);
    }
    MyPlayer.update(dt);
}

void PlayerManager::draw(sf::RenderWindow& window) {
    std::vector<int> toDraw;

    for (const auto& [id, player] : otherPlayers) {
        toDraw.push_back(id); // draw�� ID�� ���� ���� ����
    }

    for (int id : toDraw) {
        if (otherPlayers.find(id) != otherPlayers.end()) {
            otherPlayers[id].draw(window); // draw �������� ���� Ȯ��
        }
    }

    MyPlayer.draw(window);
}

// --------------------------------------

void PlayerManager::handleEvent(std::string tag, std::string msg) {
    if (tag == "MOVE") {
        std::istringstream iss(msg);
        std::string result, dirStr;
        int id;
        iss >> result >> id >> dirStr;

        Direction dir;
        if (dirStr == "UP") dir = Direction::Up;
        else if (dirStr == "DOWN") dir = Direction::Down;
        else if (dirStr == "LEFT") dir = Direction::Left;
        else if (dirStr == "RIGHT") dir = Direction::Right;
        else dir = Direction::None;

        if (result == "TRUE") {
            int x, y;
            iss >> x >> y;
            if (id == NetworkManager::getInstance().getSocketID()) {
                MyPlayer.move({ x, y }, dir);
            }
            else {
                if (auto* player = getPlayer(id)) {
                    player->move({ x, y }, dir);
                }
            }
        }
        else if (result == "FALSE") {
            if (id == NetworkManager::getInstance().getSocketID()) {
                MyPlayer.setCurDir(dirStr);
            }
            else {
                if (auto* player = getPlayer(id)) {
                    player->setCurDir(dirStr);
                }
            }
        }
        EventManager::getInstance().clearEvents(tag);
    }
    else if (tag == "NEWUSER") {
        std::istringstream iss(msg);
        int id, x, y, win, lose, level, exp;
        std::string name;
        std::cout << "PlayerManager : NewUser ����!" << std::endl;
        iss >> id >> name >> x >> y >> win >> lose >> level >> exp;
        if (id == NetworkManager::getInstance().getSocketID()) {}
        else { addPlayer(id, name, x, y, win, lose, level, exp); }
        EventManager::getInstance().clearEvents(tag);
    }

    else if (tag == "EXITUSER") {
        std::istringstream iss(msg);
        int id;
        iss >> id;
        std::cout << "PlayerManager : ExitUser ����!" << std::endl;
        removePlayer(id);
        EventManager::getInstance().clearEvents(tag);
    }

    else if (tag == "CHAT") {
        std::istringstream iss(msg);
        int id;
        std::string message;
        iss >> id; 
        std::cout << "id�Ľ� : " << id << std::endl;
        std::getline(iss,message);
        if (!message.empty() && message[0] == ' ') {
            message = message.substr(1); // �� ���� ����
        }
        std::cout << "���� ���� �޽���" << message << std::endl;
        if (id == NetworkManager::getInstance().getSocketID())
        {
            MyPlayer.showSpeechBubble(message);
        }
        else
        {
            otherPlayers[id].showSpeechBubble(message);
        }
        EventManager::getInstance().clearEvents(tag);
    }
}

void PlayerManager::removePlayer(int id) {
    otherPlayers.erase(id);
}

Player* PlayerManager::getPlayer(int id) {
    auto it = otherPlayers.find(id);
    if (it != otherPlayers.end()) return &it->second;
    return nullptr;
}

Player& PlayerManager::getMyPlayer() {
    return MyPlayer;
}

