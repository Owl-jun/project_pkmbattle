#include "pch.h"
#include "GameManager.h"
#include "KeyManager.h"
#include "SceneManager.hpp"
#include "OpeningScene.hpp"
#include "TitleScene.hpp"
#include "worldScene.hpp"
#include "LoginScene.hpp"
#include "OpeningScene.hpp"
#include "TimeManager.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include "NetworkManager.hpp"

GameManager::GameManager()
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE") // 윈도우 타이틀 및 해상도설정
    , muteControl({ 720,500 })
{
}

Player& GameManager::getPlayer() {
    return *player;
}

GameManager& GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

sf::RenderWindow& GameManager::getWindow()
{
    return window;
}

void GameManager::init() {
    // 네트워크연결 , 테스트 완료 후 로그인시 연결하는걸로 변경
    NetworkManager::getInstance();
    NetworkManager::getInstance().connect("localhost", "9000");   // 연결문제 해결되면 주석해제
    std::string response;
    while (response.empty()) {
        response = NetworkManager::getInstance().receive();
    }
    int myId = -1;
    if (!response.empty()) {
        std::istringstream iss(response);
        std::string type;
        iss >> type;

        if (type == "ID") {
            iss >> myId;
            NetworkManager::getInstance().setMyId(myId);
            std::cout << "[Client] 내 ID는 " << NetworkManager::getInstance().getMyId() << "\n";
        }
        else {
            std::cout << "[Client] Unknown server response: " << response << "\n";
        }
    }
    // -------------------
    SoundManager::getInstance().playMusic("C:/Source/project_pkmbattle/Client/assets/track1.mp3");  // 사운드매니저
    ResourceManager::getInstance().init();
    player = std::make_unique<Player>();
    SceneManager::getInstance().registerScene("opening", new OpeningScene());
    SceneManager::getInstance().registerScene("title", new TitleScene());
    SceneManager::getInstance().registerScene("login", new LoginScene());
    SceneManager::getInstance().registerScene("world", new worldScene());
    SceneManager::getInstance().changeScene("opening");    // 초기화면 설정
}

void GameManager::update() { 
    TimeManager::getInstance().tick();
    KeyManager::getInstance().update();
    muteControl.update(window);
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
        KeyManager::getInstance().handleEvent(*event);
        muteControl.handleEvent(*event,window);
        SceneManager::getInstance().handleInput(*event, window);
    }
    SceneManager::getInstance().update(window);
}

void GameManager::render() {  
    window.clear();
    SceneManager::getInstance().render(window);
    if (!(SceneManager::getInstance().getCurScene() == SceneManager::getInstance().getScene("world")))
    {
        muteControl.render(window);
    }
    window.display();
}

void GameManager::run() {
    init();
    while (window.isOpen()) {
        update();
        render();
    }
}
