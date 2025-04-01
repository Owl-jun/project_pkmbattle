#pragma once

class GameManager {
private:
    // ∞‘¿” ¿©µµøÏ
    sf::RenderWindow window;
      
    // ΩÃ±€≈Ê
    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

public:

    static GameManager& getInstance();
    sf::RenderWindow& getWindow();
    void init();
    void handleInput();
    void handleEvent(std::string tag, std::string msg);
    void update();
    void render();
    void run();
};
