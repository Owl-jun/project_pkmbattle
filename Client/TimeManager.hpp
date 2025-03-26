#pragma once
#include "pch.h"

class TimeManager {
private:
    sf::Clock clock;
    float deltaTime = 0.f;
    TimeManager() = default;

public:
    static TimeManager& getInstance() {
        static TimeManager instance;
        return instance;
    }

    void update() {
        deltaTime = clock.restart().asSeconds();
    }

    float getDeltaTime() const {
        return deltaTime;
    }

};
