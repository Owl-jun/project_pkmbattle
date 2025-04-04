#include "pch.h"
#include "PlayerManager.hpp"
#include "Player.hpp"

void PlayerManager::addPlayer(int id, std::string nickname, int _x, int _y, int _win, int _lose, int _level, int _exp) {
    if (otherPlayers.contains(id)) return;
    Player p(nickname, _x, _y, _win, _lose, _level, _exp);
    otherPlayers[id] = p;
    std::cout << "[PlayerManager] : addPlayer 실행 완료!" << std::endl;
}

void PlayerManager::make_MyPlayer(int id, std::string nickname, int _x , int _y, int _win , int _lose, int _level, int _exp) 
{

    MyPlayer = Player(nickname, _x, _y, _win, _lose, _level, _exp);
    std::cout << "[PlayerManager] : make_MyPlayer 실행 완료! id : " << std::to_string(id) << std::endl;
}

// --------------------------------------
// 3종세트
void PlayerManager::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    MyPlayer.handleInput(event, window, true);
}

void PlayerManager::update(float dt) {
    // 수신 패킷이름 지정
    for (const std::string& tag : { "MOVE" , "NEWUSER" , "EXITUSER", "CHAT" , "CHCOLOR", "GETCAP"}) {
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
        toDraw.push_back(id); // draw용 ID만 먼저 따로 저장
    }

    for (int id : toDraw) {
        if (otherPlayers.find(id) != otherPlayers.end()) {
            otherPlayers[id].draw(window); // draw 시점에서 존재 확인
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

        bool done = false;
        if (result == "TRUE") {
            int x, y;
            iss >> x >> y;
            if (id == NetworkManager::getInstance().getSocketID()) {
                if (!MyPlayer.isMove()) {
                    MyPlayer.move({ x, y }, dir);
                    done = true;
                }
            }
            else {
                if (auto* player = getPlayer(id)) {
                    if (!player->isMove()) {
                        player->move({ x, y }, dir);
                        done = true;
                    }
                }
            }
        }
        else if (result == "FALSE") {
            if (id == NetworkManager::getInstance().getSocketID()) {
                MyPlayer.setCurDir(dirStr);
                done = true;
            }
            else {
                if (auto* player = getPlayer(id)) {
                    player->setCurDir(dirStr);
                    done = true;
                }
            }
        }
        if (done) { EventManager::getInstance().clearEvents(tag); }
    }
    else if (tag == "NEWUSER") {
        std::istringstream iss(msg);
        int id, x, y, win, lose, level, exp;
        std::string name;
        //std::cout << "PlayerManager : NewUser 수신!" << std::endl;
        iss >> id >> name >> x >> y >> win >> lose >> level >> exp;
        if (id == NetworkManager::getInstance().getSocketID()) {}
        else { addPlayer(id, name, x, y, win, lose, level, exp); }
        EventManager::getInstance().clearEvents(tag);
    }

    else if (tag == "EXITUSER") {
        std::istringstream iss(msg);
        int id;
        iss >> id;
        //std::cout << "PlayerManager : ExitUser 수신!" << std::endl;
        removePlayer(id);
        EventManager::getInstance().clearEvents(tag);
    }

    else if (tag == "CHAT") {
        std::istringstream iss(msg);
        int id;
        std::string message;
        iss >> id; 
        //std::cout << "id파싱 : " << id << std::endl;
        std::getline(iss,message);
        if (!message.empty() && message[0] == ' ') {
            message = message.substr(1); // 앞 공백 제거
        }
        //std::cout << "현재 도착 메시지" << message << std::endl;
        chatting.addMessage(message);
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

    else if (tag == "CHCOLOR") {
        
        std::istringstream iss(msg);
        int id, set;
        iss >> id >> set;

        if (id == NetworkManager::getInstance().getSocketID()) { getMyPlayer().setColor(set); }
        else {
            otherPlayers[id].setColor(set);
        }
        EventManager::getInstance().clearEvents(tag);
    }
    else if (tag == "GETCAP") {
        std::istringstream iss(msg);
        int id;
        std::string com;
        iss >> id >> com;

        if (com == "GET")
        {
            PlayerManager::getInstance().setCapHolderId(id);
        }
        else if (com == "SEND")
        {
            int recvID;
            iss >> recvID;
            if (recvID != -1) {
                PlayerManager::getInstance().setCapHolderId(recvID);
            }
        }
        else if (com == "LOST")
        {
            PlayerManager::getInstance().setCapHolderId(-1);
        }
        EventManager::getInstance().clearEvents(tag);
    }
}

void PlayerManager::removePlayer(int id) {
    if (capHolderId == id) {
        capHolderId = -1;
    }
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

std::unordered_map<int, Player>& PlayerManager::getPlayers()
{
    return otherPlayers;
}

