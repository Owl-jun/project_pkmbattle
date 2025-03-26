#pragma once
#include "pch.h"
#include "BaseScene.hpp"


class SceneManager {
private:
    BaseScene* currentScene = nullptr;

    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    static SceneManager& getInstance() {
        static SceneManager instance;
        return instance;
    }

    void changeScene(BaseScene* newScene) {
        if (currentScene)
            delete currentScene;
        currentScene = newScene;
        currentScene->init();
    }

    void update(sf::RenderWindow& window) {
        if (currentScene)
            currentScene->update(window);
    }

    void render(sf::RenderWindow& window) {
        if (currentScene)
            currentScene->render(window);
    }
};
