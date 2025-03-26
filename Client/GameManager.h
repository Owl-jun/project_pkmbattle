#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

class GameManager {
private:
    sf::RenderWindow window;
    Player player;

    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

public:
    static GameManager& getInstance();
    sf::RenderWindow& getWindow();
    void init();
    void update();
    void render();
    void run();
};
