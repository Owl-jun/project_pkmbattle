#pragma once
#include "UImuteControl.hpp"
#include "Player.h"


class GameManager {
private:
    sf::RenderWindow window;
    std::unique_ptr<Player> player;
    UImuteControl muteControl;

    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

public:
    Player& getPlayer();
    static GameManager& getInstance();
    sf::RenderWindow& getWindow();
    void init();
    void update();
    void render();
    void run();
};
