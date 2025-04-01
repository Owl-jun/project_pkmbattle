
#include "DBManager.hpp"
#include <iostream>
#include <asio.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>

using asio::ip::tcp;

// ---- Collision Map ----
const std::vector<std::vector<int>> collisionMap = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} 
};

const int MAP_WIDTH = collisionMap[0].size();
const int MAP_HEIGHT = collisionMap.size();

// --- DB M 초기화 //
DBManager DBM;

// --- 상호작용 관련 구조체 //
struct FightInteraction {
    int id1, id2;
    std::string choice1 = ""; // "FIGHT" or "RUN"
    std::string choice2 = "";

};

std::vector<FightInteraction> ongoingFights;

bool operator==(const FightInteraction& a, const FightInteraction& b) {
    return (a.id1 == b.id1 && a.id2 == b.id2) || (a.id1 == b.id2 && a.id2 == b.id1);
}


// --- 방향 정의 ---
std::unordered_map<std::string, std::pair<int, int>> directionOffset = {
    {"UP", {0, -1}},
    {"DOWN", {0, 1}},
    {"LEFT", {-1, 0}},
    {"RIGHT", {1, 0}}
};


// --- 글로벌 상태 ---
std::unordered_map<int, Player> players;
std::unordered_map<int, std::shared_ptr<tcp::socket>> clientSockets;
std::unordered_map<int, string> ids;
std::unordered_set<string> users;
std::mutex playerMutex;
int nextPlayerId = 1;


// --- 이동 가능 여부 확인 ---
bool canMoveTo(int x, int y) {
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && collisionMap[y][x] == 0);
}

// --- 브로드캐스트 ---
void broadcastPlayerStates() {
    std::string response = "PLAYERS\n";
    {
        std::lock_guard<std::mutex> lock(playerMutex);
        for (const auto& [id, player] : players) {
            response += std::to_string(id) + " " + std::to_string(player.x) + " " + std::to_string(player.y) + " DOWN" + '\n';
        }
    }
    for (const auto& [id, sock] : clientSockets) {
        std::cout << "broadcast: " << response;
        asio::write(*sock, asio::buffer(response));
    }
}


// --- 메시지 처리 ---
void processMessage(const std::string& msg, int playerId) {
    std::istringstream iss(msg);
    std::string command, dir;
    std::string chatmsg;
    std::string id, password;
    iss >> command;

    /* 이형우 강석준 황석준 */
    // 로그인 로직추가

    //--------------------------------------
    if (command == "EXIT") {
        std::string response;

        std::lock_guard<std::mutex> lock(playerMutex);
        if (players.count(playerId)) {
            try {
                const auto& p = players[playerId];
                DBM.savePlayer(p.id, p.x, p.y, p.win, p.lose, p.level, p.EXP);
                response = "EXIT_OK\n";
            }
            catch (std::exception& e) {
                response = "EXIT_FAIL\n";
            }
        }
        // 클라이언트에게 결과 전송
        if (clientSockets.count(playerId)) {
            asio::write(*clientSockets[playerId], asio::buffer(response));
        }
    }
    //--------------------------------------

    else if (command == "LOGIN") {
        iss >> id >> password;
        std::cout << id << password << std::endl;
        string response;
        if(DBM.canLogin(id, password)){
            Player player = DBM.loadPlayer(id);
            if (!player.isEmpty()) {
                {
                    std::lock_guard<std::mutex> lock(playerMutex);
                    players[playerId] = player;
                }
                if (users.find(id) == users.end())
                {
                    ids[playerId] = id;
                    users.insert(id);
                    response = "LOGIN TRUE " + std::to_string(player.x) + ' ' + std::to_string(player.y) + '\n';
                    asio::write(*clientSockets[playerId], asio::buffer(response));
                    broadcastPlayerStates();
                }
                else 
                {
                    response = "LOGIN alreadyLoginPlayer\n";
                    asio::write(*clientSockets[playerId], asio::buffer(response));
                }
            }
        }
        else {
            response = "LOGIN FALSE\n";
            cout << response << endl;
            asio::write(*clientSockets[playerId], asio::buffer(response));
        }
        
    }

    else if (command == "MOVE") {
        iss >> dir;
        if (directionOffset.count(dir)) {
            std::lock_guard<std::mutex> lock(playerMutex);
            auto& p = players[playerId];
            p.dir = dir;
            int nx = p.x + directionOffset[dir].first;
            int ny = p.y + directionOffset[dir].second;

            // 플레이어 간 충돌 검사
            bool blocked = false;
            for (const auto& [otherId, otherPlayer] : players) {
                if (otherId != playerId && otherPlayer.x == nx && otherPlayer.y == ny) {
                    blocked = true;
                    break;
                }
            }

            if (!blocked && canMoveTo(nx, ny)) {
                p.x = nx;
                p.y = ny;
            }

            std::string response;
            response = "PLAYERS\n";
            for (const auto& [id, player] : players) {
                response += std::to_string(id) + " " + std::to_string(player.x) + " " + std::to_string(player.y) + " " + player.dir + "\n";
            }

            for (const auto& [id, sock] : clientSockets) {
                asio::write(*sock, asio::buffer(response));
            }

        }
    }

    else if (command == "CHAT") {
       
        std::getline(iss, chatmsg); // 공백 포함 전체 읽기
        if (!chatmsg.empty() && chatmsg[0] == ' ') chatmsg.erase(0, 1);  // 앞에 공백 제거

        std::string response = "CHAT " + std::to_string(playerId) + " " + ids[playerId] + " " + chatmsg + "\n";
        for (const auto& [id, sock] : clientSockets) {
            asio::write(*sock, asio::buffer(response));
        }
    }

    // 상호작용 명령 처리
    else if (command == "INTERACT") {
        std::cout << "Interact 호출" << std::endl;
        int targetId;
        iss >> targetId;

        ongoingFights.push_back({ playerId, targetId });

        std::string response = "INTERACTION " + std::to_string(playerId) + " " + std::to_string(targetId) + "\n";
        for (auto& [id, sock] : clientSockets) {
            asio::write(*sock, asio::buffer(response));
        }
    }
    else if (command == "FIGHT_RESPONSE") {
        std::cout << "FIGHT_RESPONSE 호출" << std::endl;

        int senderId;
        std::string choice;
        iss >> senderId >> choice;

        for (auto& fight : ongoingFights) {
            if (fight.id1 == senderId || fight.id2 == senderId) {
                if (fight.id1 == senderId) fight.choice1 = choice;
                else fight.choice2 = choice;

                // 누군가 도망간 경우
                if (choice == "RUN") {
                    std::string msg = "FIGHT_RESULT " + std::to_string(fight.id1) + " " + std::to_string(fight.id2) + " CANCELED\n";
                    asio::write(*clientSockets[fight.id1], asio::buffer(msg));
                    asio::write(*clientSockets[fight.id2], asio::buffer(msg));
                    ongoingFights.erase(std::remove(ongoingFights.begin(), ongoingFights.end(), fight), ongoingFights.end());
                    break;
                }

                // 둘 다 싸운다 선택
                if (fight.choice1 == "FIGHT" && fight.choice2 == "FIGHT") {
                    std::string msg = "FIGHT_RESULT " + std::to_string(fight.id1) + " " + std::to_string(fight.id2) + " READY\n";
                    asio::write(*clientSockets[fight.id1], asio::buffer(msg));
                    asio::write(*clientSockets[fight.id2], asio::buffer(msg));
                    ongoingFights.erase(std::remove(ongoingFights.begin(), ongoingFights.end(), fight), ongoingFights.end());
                    break;
                }

                // 대기 상태 (상대가 아직 선택 안 한 경우)
            }
        }
    }


    iss.clear();
}


// --- 클라이언트 처리 스레드 ---
void handleClient(int playerId, std::shared_ptr<tcp::socket> socket) {
    char temp[128];
    std::string buffer;

    try {
        // 클라이언트에게 본인 ID 전송
        std::string idMsg = "ID " + std::to_string(playerId) + "\n";
        asio::write(*socket, asio::buffer(idMsg));

        while (true) {
            asio::error_code ec;
            size_t len = socket->read_some(asio::buffer(temp), ec);
            if (ec == asio::error::eof) break;
            if (ec) throw asio::system_error(ec);

            buffer.append(temp, len);
            size_t pos;
            while ((pos = buffer.find('\n')) != std::string::npos) {
                std::string line = buffer.substr(0, pos + 1);
                buffer.erase(0, pos + 1);
                processMessage(line, playerId);
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "[Server] Player " << playerId << " disconnected: " << e.what() << "\n";
        
        // 연결 종료 시 클린업
        std::lock_guard<std::mutex> lock(playerMutex);
        users.erase(ids[playerId]);
        ids.erase(playerId);
        players.erase(playerId);
        clientSockets.erase(playerId);
    }

    std::string response = "PLAYERS\n";
    {
        std::lock_guard<std::mutex> lock(playerMutex);
        for (const auto& [id, player] : players) {
            response += std::to_string(id) + " " + std::to_string(player.x) + " " + std::to_string(player.y) + "\n";
        }
    }
    for (const auto& [id, sock] : clientSockets) {
        asio::write(*sock, asio::buffer(response));
    }

}

// --- 서버 메인 ---
int main() {
    try {
        asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 9000));
        std::cout << "[Server] 대기 중...\n";

        while (true) {
            auto socket = std::make_shared<tcp::socket>(io);
            acceptor.accept(*socket);

            std::lock_guard<std::mutex> lock(playerMutex);
            int id = nextPlayerId++;
            
            clientSockets[id] = socket;

            std::cout << "[Server] Player " << id << " 접속!\n";

            std::thread(handleClient, id, socket).detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << "[Server 오류] " << e.what() << "\n";
    }

    return 0;
}