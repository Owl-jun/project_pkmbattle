#include "pch.h"
#include "NetworkManager.hpp"


using asio::ip::tcp;
NetworkManager::NetworkManager() {
    // 소켓 생성
    socket = std::make_shared<tcp::socket>(io_context);
}


// --------------------------------------------------------------
// get , setter

int NetworkManager::getSocketID() const { return socketID; }

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

std::shared_ptr<tcp::socket> NetworkManager::getSocket() {
    return socket;
}
// --------------------------------------------------------------



// --------------------------------------------------------------
// 연결 및 송 수신
void NetworkManager::connect(const std::string& ip, const std::string& port) {
    tcp::resolver resolver(io_context);
    asio::error_code ec;
    auto endpoints = resolver.resolve(ip, port, ec);
    if (ec) {
        std::cerr << "[Connect Error] resolve 실패: " << ec.message() << "\n";
        return;
    }

    asio::connect(*socket, endpoints, ec);
    if (ec) {
        std::cerr << "[Connect Error] 연결 실패: " << ec.message() << "\n";
    }
    else {
        std::cout << "[Connect] 서버 연결 성공!\n";
    }
}

void NetworkManager::send(const std::string& data) {
    asio::write(*socket, asio::buffer(data));
}

std::string NetworkManager::receive_block(int timeoutSeconds) {
    socket->non_blocking(false);
    asio::streambuf buf;
    asio::error_code read_ec;
    asio::steady_timer timer(io_context);
    bool timeout = false;

    if (timeoutSeconds > 0) {
        timer.expires_after(std::chrono::seconds(timeoutSeconds));
        timer.async_wait([&](const asio::error_code& ec) {
            if (!ec) {
                timeout = true;
                socket->cancel(); // 타임아웃 시도
            }
        });
    }

    asio::async_read_until(*socket, buf, "\n", [&](const asio::error_code& ec, size_t) {
        read_ec = ec;
        if (timeoutSeconds > 0)
            timer.cancel();
    });

    io_context.run();
    io_context.restart();

    if (timeout || (read_ec && read_ec != asio::error::eof)) {
        std::cerr << "[Receive Error] " << read_ec.message() << "\n";
        return "";
    }

    std::istream is(&buf);
    std::string line;
    std::getline(is, line);
    return line;
}
// --------------------------------------------------------------


// --------------------------------------------------------------
// 스레드 관련
void NetworkManager::startReceiveLoop() {
    running = true;
    receiveThread = std::thread(&NetworkManager::receiveLoop, this);
}

void NetworkManager::stopReceiveLoop() {
    running = false;
    if (receiveThread.joinable())
        receiveThread.join();
}

void NetworkManager::receiveLoop() {
    while (running) {
        std::string msg = receive_block(5); // 5초 타임아웃
        if (!msg.empty()) {
            pushMessage(msg);
        }
    }
}
// --------------------------------------------------------------


// --------------------------------------------------------------
// 메시지 큐 관련 메서드
void NetworkManager::pushMessage(const std::string& msg) {
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueue.push(msg);
}

std::string NetworkManager::popMessage() {
    std::lock_guard<std::mutex> lock(queueMutex);
    if (messageQueue.empty()) return "";
    std::string msg = messageQueue.front();
    messageQueue.pop();
    return msg;
}

bool NetworkManager::hasMessage() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return !messageQueue.empty();
}
// --------------------------------------------------------------








