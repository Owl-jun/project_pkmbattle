#pragma once
#include "pch.h"
#include "BaseScene.hpp"


class SceneManager {
private:
    BaseScene* currentScene = nullptr;
    BaseScene* pendingScene = nullptr;

    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    static SceneManager& getInstance() {
        static SceneManager instance;
        return instance;
    }

    void changeScene(BaseScene* newScene) {
        pendingScene = newScene;
    }

    void applyPendingScene() {
        if (pendingScene) {
            if (currentScene) delete currentScene;
            currentScene = pendingScene;
            currentScene->init();
            pendingScene = nullptr;
        }
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) {
        if (currentScene)
            currentScene->handleInput(event, window);  // 새로 추가
    }

    void update(sf::RenderWindow& window) {
        if (currentScene)
            currentScene->update(window);
        applyPendingScene();
    }

    void render(sf::RenderWindow& window) {
        if (currentScene)
            currentScene->render(window);
    }
};
