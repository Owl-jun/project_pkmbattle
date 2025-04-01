#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>
#include <memory>
#include <string>
#include <system_error>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

using asio::ip::tcp;

class NetworkManager {
private:
    asio::io_context io_context;
    std::shared_ptr<tcp::socket> socket;
    int socketID = -1;
    std::queue<std::string> messageQueue;
    std::mutex queueMutex;
    std::thread receiveThread;
    bool running = false;

    NetworkManager(); // 싱글톤

public:
    int getSocketID() const;
    static NetworkManager& getInstance();
    std::shared_ptr<tcp::socket> getSocket();

    void connect(const std::string& ip, const std::string& port);
    void send(const std::string& data);
    std::string receive_block(int timeout = 0); 

    void startReceiveLoop();               
    void stopReceiveLoop();                
    void receiveLoop();                    

    // 메시지 큐 관련
    void pushMessage(const std::string& msg);
    std::string popMessage();
    bool hasMessage();
};
