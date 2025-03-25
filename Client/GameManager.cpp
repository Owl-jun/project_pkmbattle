#include "pch.h"
#include "GameManager.h"
#include "KeyManager.h"
#include "SceneManager.hpp"
#include "TitleScene.hpp"

GameManager::GameManager()
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE")
{
}

GameManager& GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::init() {
    SceneManager::getInstance().changeScene(new TitleScene());
}

void GameManager::update() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        KeyManager::getInstance().handleEvent(*event);
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
