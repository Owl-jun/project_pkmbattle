
#include "DBManager.hpp"
#include <iostream>
#include <asio.hpp>
#include <string>
#include <unordered_map>
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
    std::string id, password;
    iss >> command;

    /* 이형우 강석준 */
    // 로그인 로직추가

    //--------------------------------------
    if (command == "EXIT") {
        std::string exitResponse;

        std::lock_guard<std::mutex> lock(playerMutex);
        if (players.count(playerId)) {
            try {
                const auto& p = players[playerId];
                DBM.savePlayer(p.id, p.x, p.y, p.win, p.lose, p.level, p.EXP);
                exitResponse = "EXIT_OK\n";
            }
            catch (std::exception& e) {
                exitResponse = "EXIT_FAIL\n";
            }
        }
        // 클라이언트에게 결과 전송
        if (clientSockets.count(playerId)) {
            asio::write(*clientSockets[playerId], asio::buffer(exitResponse));
        }
    }
    //--------------------------------------

    if (command == "LOGIN") {
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

                response = "LOGIN TRUE " + std::to_string(player.x) + ' ' + std::to_string(player.y) + '\n';
                asio::write(*clientSockets[playerId], asio::buffer(response));
                broadcastPlayerStates();
            }
        }
        else {
            response = "LOGIN FALSE\n";
            cout << response << endl;
            asio::write(*clientSockets[playerId], asio::buffer(response));
        }
        
    }

    if (command == "MOVE") {
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
                cout << player.x << " , " << player.y << endl;
                response += std::to_string(id) + " " + std::to_string(player.x) + " " + std::to_string(player.y) + " " + player.dir + "\n";
            }

            for (const auto& [id, sock] : clientSockets) {
                asio::write(*sock, asio::buffer(response));
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