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

// --- ���� ���� ---
std::unordered_map<std::string, std::pair<int, int>> directionOffset = {
    {"UP", {0, -1}},
    {"DOWN", {0, 1}},
    {"LEFT", {-1, 0}},
    {"RIGHT", {1, 0}}
};

// --- �÷��̾� ����ü ---
struct Player {
    int x;
    int y;
};

// --- �۷ι� ���� ---
std::unordered_map<int, Player> players;
std::unordered_map<int, std::shared_ptr<tcp::socket>> clientSockets;
std::mutex playerMutex;
int nextPlayerId = 1;

// --- �̵� ���� ���� Ȯ�� ---
bool canMoveTo(int x, int y) {
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && collisionMap[y][x] == 0);
}

// --- �޽��� ó�� ---
void processMessage(const std::string& msg, int playerId) {
    std::istringstream iss(msg);
    std::string command, dir;
    iss >> command >> dir;

    if (command == "MOVE" && directionOffset.count(dir)) {
        std::lock_guard<std::mutex> lock(playerMutex);
        auto& p = players[playerId];
        int nx = p.x + directionOffset[dir].first;
        int ny = p.y + directionOffset[dir].second;

        // �÷��̾� �� �浹 �˻�
        bool blocked = false;
        for (const auto& [otherId, otherPlayer] : players) {
            if (otherId != playerId && otherPlayer.x == nx && otherPlayer.y == ny) {
                blocked = true;
                break;
            }
        }

        bool moved = false;
        if (!blocked && canMoveTo(nx, ny)) {
            p.x = nx;
            p.y = ny;
            moved = true;
        }

        if (moved) {
            std::string response = "PLAYERS\n";
            for (const auto& [id, player] : players) {
                response += std::to_string(id) + " " + std::to_string(player.x) + " " + std::to_string(player.y) + "\n";
            }

            for (const auto& [id, sock] : clientSockets) {
                asio::write(*sock, asio::buffer(response));
            }
        }
    }
}

// --- Ŭ���̾�Ʈ ó�� ������ ---
void handleClient(int playerId, std::shared_ptr<tcp::socket> socket) {
    char temp[128];
    std::string buffer;

    try {
        // Ŭ���̾�Ʈ���� ���� ID ����
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
        // ���� ���� �� Ŭ����
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

// --- ���� ���� ---
int main() {
    try {
        asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 9000));
        std::cout << "[Server] ��� ��...\n";

        while (true) {
            auto socket = std::make_shared<tcp::socket>(io);
            acceptor.accept(*socket);

            std::lock_guard<std::mutex> lock(playerMutex);
            int id = nextPlayerId++;
            players[id] = { 2, 39 };  // �ʱ� ��ġ
            clientSockets[id] = socket;

            std::cout << "[Server] Player " << id << " ����!\n";

            std::thread(handleClient, id, socket).detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << "[Server ����] " << e.what() << "\n";
    }

    return 0;
}