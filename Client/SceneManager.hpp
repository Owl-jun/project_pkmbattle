#pragma once
#include "baseScene.hpp"


class SceneManager {
private:
    std::unordered_map<std::string, BaseScene*> scenes;
    BaseScene* currentScene = nullptr;
    std::string pendingSceneKey;

    // 싱글톤
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    static SceneManager& getInstance() {
        static SceneManager instance;
        return instance;
    }

    // 서버 패킷 핸들러
    void handleEvent(std::string tag, std::string msg);

    // ----------------------------------------------------------------------
    // 게임루프
    void handleInput(const sf::Event& event, sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    // ----------------------------------------------------------------------


    // --------------------------------------------------------------
    // 씬 등록, 전환 관련
    void registerAll();
    void registerScene(const std::string& key, BaseScene* scene);
    void changeScene(const std::string& key);
    void applyPendingScene();
    // --------------------------------------------------------------


    // ---------------------------------------------------------------
    // 씬 게터
    BaseScene* getScene(const std::string& key);
    BaseScene* getCurScene();
    // ---------------------------------------------------------------


    // ---------------------------------------------------------------
    // 메모리관리
    void cleanup();

    ~SceneManager();
    
    // ---------------------------------------------------------------

};