
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
std::unordered_map<int, Player> players;                                // 플레이어 객체 저장
std::unordered_map<int, std::shared_ptr<tcp::socket>> clientSockets;    // 소켓 저장소
std::unordered_map<int, std::string> users;                                       // 접속 유저
std::mutex playerMutex;
int nextPlayerId = 1;

// --- 이동 가능 여부 확인 ---
bool canMoveTo(int x, int y) {
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && collisionMap[y][x] == 0);
}

// --- LOGIN 태그 헬퍼함수 ---
std::string makeLoginSuccessResponse(int playerId, const std::unordered_map<int, Player>& players) {
    std::ostringstream oss;
    const auto& myPlayer = players.at(playerId);

    // 첫 줄: LOGIN TRUE
    oss << "LOGIN TRUE ";

    // 내 정보
    oss << playerId << " "
        << myPlayer.name << " "
        << myPlayer.x << " "
        << myPlayer.y << " "
        << myPlayer.win << " "
        << myPlayer.lose << " "
        << myPlayer.level << " "
        << myPlayer.EXP << "\n";

    // 나 이외의 모든 플레이어 정보
    for (const auto& [id, p] : players) {
        if (id == playerId) continue;
        oss << id << " "
            << p.name << " "
            << p.x << " "
            << p.y << " "
            << p.win << " "
            << p.lose << " "
            << p.level << " "
            << p.EXP << "\n";
    }

    return oss.str();
}


// --- 메시지 처리 ---
void processMessage(const std::string& msg, int playerId) {
    std::istringstream iss(msg);
    std::string command, dir;
    std::string chatmsg;
    std::string id, password;
    iss >> command;

    //--------------------------------------
    if (command == "EXIT") {
        std::cout << "EXIT 수신 메시지 처리시작" << std::endl;
        std::string response;
        std::string broadRes;
        std::lock_guard<std::mutex> lock(playerMutex);
        if (players.count(playerId)) {
            try {
                const auto& p = players[playerId];
                DBM.savePlayer(p.id, p.x, p.y, p.win, p.lose, p.level, p.EXP);
                response = "EXIT " + std::to_string(playerId) + " EXIT_OK\n";
                broadRes = "EXITUSER " + std::to_string(playerId) + '\n';
                asio::write(*clientSockets[playerId], asio::buffer(response));
                // 브로드 캐스트
                for (const auto& [id, sock] : clientSockets) {
                    asio::write(*sock, asio::buffer(broadRes));
                }
            }
            catch (std::exception& e) {
                response = "EXIT EXIT_FAIL\n";
            }
        }
        else { asio::write(*clientSockets[playerId], asio::buffer("EXIT " + std::to_string(playerId) + " EXIT_OK\n")); }
    }

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
                if (users[playerId].empty()) {
                    users[playerId] = id;

                    // 플레이어 목록에 추가
                    players[playerId] = player;

                    std::string response = makeLoginSuccessResponse(playerId, players);

                    asio::write(*clientSockets[playerId], asio::buffer(response));

                    // NEWUSER 브로드캐스트 (다른 플레이어에게 알림)
                    std::string broadRes = "NEWUSER " + std::to_string(playerId) + ' '
                        + player.name + ' '
                        + std::to_string(player.x) + ' '
                        + std::to_string(player.y) + ' '
                        + std::to_string(player.win) + ' '
                        + std::to_string(player.lose) + ' '
                        + std::to_string(player.level) + ' '
                        + std::to_string(player.EXP) + '\n';

                    for (const auto& [id, sock] : clientSockets) {
                        asio::write(*sock, asio::buffer(broadRes));
                    }
                }

                else 
                {
                    response = "LOGIN " + std::to_string(playerId) + " EXIST\n";
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

        std::string response = "CHAT " + std::to_string(playerId) + " " + chatmsg + "\n";
        for (const auto& [id, sock] : clientSockets) {
            asio::write(*sock, asio::buffer(response));
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
        users.erase(playerId);
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

// 1:1 스레드할당 , 플레이어 500명이 들어왔다 => 스레드 500개임
// 4개 만 만들어서 , 몇천명 수신가능