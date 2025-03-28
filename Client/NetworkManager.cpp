#include "pch.h"
#include "NetworkManager.hpp"
#include <iostream>

using asio::ip::tcp;

NetworkManager* NetworkManager::instance = nullptr;

NetworkManager::NetworkManager() {
    socket = std::make_shared<tcp::socket>(io_context);
    std::cout << "서버 연결 완료\n";
}

NetworkManager& NetworkManager::getInstance() {
    if (!instance) {
        instance = new NetworkManager();
    }
    return *instance;
}

void NetworkManager::connect(const std::string& ip, const std::string& port) {
    /*tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(ip, port);
    asio::connect(*socket, endpoints);
    std::cout << "서버 연결 완료\n";*/

    socket = std::make_shared<tcp::socket>(io_context); // 소켓 새로 생성
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(ip, port);
    asio::connect(*socket, endpoints);
}

void NetworkManager::send(const std::string& data) {
    asio::write(*socket, asio::buffer(data));
}

std::string NetworkManager::receive() {
    char buffer[1024];
    asio::error_code ec;
    size_t len = socket->read_some(asio::buffer(buffer), ec);

    if (ec && ec != asio::error::eof) {
        std::cerr << "[Network Error] receive 실패: " << ec.message() << "\n";
        return "";
    }

    //if (ec && ec != asio::error::eof) throw asio::system_error(ec);
    return std::string(buffer, len);
}

std::shared_ptr<tcp::socket> NetworkManager::getSocket() {
    return socket;
}
