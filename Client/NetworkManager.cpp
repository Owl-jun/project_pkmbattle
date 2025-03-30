#include "pch.h"
#include "NetworkManager.hpp"
#include <iostream>

using asio::ip::tcp;

NetworkManager* NetworkManager::instance = nullptr;

NetworkManager::NetworkManager() {
    socket = std::make_shared<tcp::socket>(io_context);
}

NetworkManager& NetworkManager::getInstance() {
    if (!instance) {
        instance = new NetworkManager();
    }
    return *instance;
}

void NetworkManager::connect(const std::string& ip, const std::string& port) {
    socket = std::make_shared<tcp::socket>(io_context);
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
void NetworkManager::setMyId(int _id) { myId = _id; }
int NetworkManager::getMyId() const { return myId; }

void NetworkManager::send(const std::string& data) {
    asio::write(*socket, asio::buffer(data));
}

std::string NetworkManager::receive() {
    socket->non_blocking(true); 

    char buffer[1024];
    asio::error_code ec;
    size_t len = socket->read_some(asio::buffer(buffer), ec);

    if (ec == asio::error::would_block) {
        return ""; 
    }
    if (ec && ec != asio::error::eof) {
        std::cerr << "[Network Error] receive 실패: " << ec.message() << "\n";
        return "";
    }

    return std::string(buffer, len);
}


std::shared_ptr<tcp::socket> NetworkManager::getSocket() {
    return socket;
}
