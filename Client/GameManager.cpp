#include "pch.h"
#include "GameManager.h"
#include "KeyManager.h"
#include "SceneManager.hpp"
#include "OpeningScene.hpp"
#include "TitleScene.hpp"
#include "worldScene.hpp"
#include "OpeningScene.hpp"
#include "TimeManager.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"

GameManager::GameManager()
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE") // ������ Ÿ��Ʋ �� �ػ󵵼���
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
    NetworkManager::getInstance();
    //NetworkManager::getInstance().connect("210.119.12.77", "9000");   // ���Ṯ�� �ذ�Ǹ� �ּ�����
    SoundManager::getInstance().playMusic("C:/Source/project_pkmbattle/Client/assets/track1.mp3");  // ����Ŵ���
    ResourceManager::getInstance().init();
    player = std::make_unique<Player>();
    SceneManager::getInstance().registerScene("opening", new OpeningScene());
    SceneManager::getInstance().registerScene("title", new TitleScene());
    SceneManager::getInstance().registerScene("login", new LoginScene());
    SceneManager::getInstance().registerScene("world", new worldScene());
    SceneManager::getInstance().changeScene("opening");    // �ʱ�ȭ�� ����
}

void GameManager::update() { 
    TimeManager::getInstance().tick();
    KeyManager::getInstance().update();
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
        KeyManager::getInstance().handleEvent(*event);
        SceneManager::getInstance().handleInput(*event, window);
    }
    SceneManager::getInstance().update(window);
}

void GameManager::render() {  
    window.clear();
    SceneManager::getInstance().render(window);
    window.display();
}

void GameManager::run() {
    init();
    while (window.isOpen()) {
        update();
        render();
    }
}
