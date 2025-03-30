#pragma once
#include "pch.h"
#include "BaseScene.hpp"
#include <unordered_map>
#include <string>

class SceneManager {
private:
    std::unordered_map<std::string, BaseScene*> scenes;
    BaseScene* currentScene = nullptr;  //
    std::string pendingSceneKey;

    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    static SceneManager& getInstance() {
        static SceneManager instance;
        return instance;
    }

    void registerScene(const std::string& key, BaseScene* scene) {
        scenes[key] = scene;
        scene->init();
    }

    // < �� �ٲٴ� �Լ� == ����� �ٲ۴�.>
    void changeScene(const std::string& key) {
        pendingSceneKey = key;
    }

    BaseScene* getScene(const std::string& key) {
        auto it = scenes.find(key);
        return (it != scenes.end()) ? it->second : nullptr;
    }

    BaseScene* getCurScene() {
        return currentScene;
    }

    void applyPendingScene() {
        if (!pendingSceneKey.empty()) {
            auto it = scenes.find(pendingSceneKey);
            if (it != scenes.end()) {
                currentScene = it->second;
            }
        }
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) {
        if (currentScene)
            currentScene->handleInput(event, window);
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

    void cleanup() {
        for (auto& [key, scene] : scenes) {
            delete scene;
        }
        scenes.clear();
    }

    ~SceneManager() {
        cleanup();
    }
};