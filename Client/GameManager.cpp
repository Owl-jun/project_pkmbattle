#include "pch.h"
#include "GameManager.h"
#include "KeyManager.h"
#include "SceneManager.hpp"
#include "OpeningScene.hpp"
#include "TimeManager.hpp"

#include "endingScene.hpp"

GameManager::GameManager()
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE") // 윈도우 타이틀 및 해상도설정
{
}

// 싱글톤
GameManager& GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

sf::RenderWindow& GameManager::getWindow()
{
    return window;
}

void GameManager::init() {
    SceneManager::getInstance().changeScene(new endingScene());    // 초기화면 설정
}

void GameManager::update() { 
    TimeManager::getInstance().update();
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
