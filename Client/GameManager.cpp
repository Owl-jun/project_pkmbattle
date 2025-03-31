#include "pch.h"
#include "GameManager.h"
#include "KeyManager.h"
#include "SceneManager.hpp"
#include "TitleScene.hpp"
#include "worldScene.hpp"
#include "LoginScene.hpp"
#include "OpeningScene.hpp"
#include "TimeManager.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include "NetworkManager.hpp"
#include "endingScene.hpp"


GameManager::GameManager()
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE") // ������ Ÿ��Ʋ �� �ػ󵵼���
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
    // ��Ʈ��ũ���� , �׽�Ʈ �Ϸ� �� �α��ν� �����ϴ°ɷ� ����
    NetworkManager::getInstance();
    NetworkManager::getInstance().connect("localhost", "9000");   // ���Ṯ�� �ذ�Ǹ� �ּ�����
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
            std::cout << "[Client] �� ID�� " << NetworkManager::getInstance().getMyId() << "\n";
        }
        else {
            std::cout << "[Client] Unknown server response: " << response << "\n";
        }
    }
    // -------------------
    SoundManager::getInstance().playMusic("C:/Source/project_pkmbattle/Client/assets/track1.mp3");  // ����Ŵ���
    ResourceManager::getInstance().init();
    player = std::make_unique<Player>();
    // ������ ��. ���� ���� �� �� ����� �ȵ�

    SceneManager::getInstance().registerScene("opening", new OpeningScene());
    SceneManager::getInstance().registerScene("title", new TitleScene());
    SceneManager::getInstance().registerScene("login", new LoginScene());
    SceneManager::getInstance().registerScene("world", new worldScene());
    SceneManager::getInstance().registerScene("ending", new endingScene());

    // --------------------------------------
    SceneManager::getInstance().changeScene("ending");
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
        update();   // ������ �ֽ�ȭ
        render();   // ȭ�鿡 ����ϴ� ���.
    }
}
