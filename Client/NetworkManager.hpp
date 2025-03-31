#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>
#include <memory>
#include <string>
#include <system_error>
using asio::ip::tcp;

class NetworkManager {
private:
    asio::io_context io_context;
    std::shared_ptr<asio::ip::tcp::socket> socket;
    static NetworkManager* instance;
    int myId = -1;
    NetworkManager(); // 생성자 private

public:
    static NetworkManager& getInstance();
    void setMyId(int);
    int getMyId() const;
    void connect(const std::string& ip, const std::string& port);
    void send(const std::string& data);
    std::string receive();
    std::string receive_line();

    std::shared_ptr<asio::ip::tcp::socket> getSocket();
};
