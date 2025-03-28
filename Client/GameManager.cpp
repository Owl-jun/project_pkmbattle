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
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE") // 윈도우 타이틀 및 해상도설정
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
    //NetworkManager::getInstance().connect("210.119.12.77", "9000");   // 연결문제 해결되면 주석해제
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
