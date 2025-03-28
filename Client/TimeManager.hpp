#pragma once
#include "pch.h"

class TimeManager {
private:
    sf::Clock clock;
    float deltaTime = 0.f;
    const float targetFrameTime = 1.f / 30.f; // FRAME
    TimeManager() = default;

public:
    static TimeManager& getInstance() {
        static TimeManager instance;
        return instance;
    }

    void tick() {
        float frameStart = clock.getElapsedTime().asSeconds();
        deltaTime = targetFrameTime;

        float elapsed = clock.getElapsedTime().asSeconds() - frameStart;
        float sleepTime = targetFrameTime - elapsed;
        if (sleepTime > 0.f)
            sf::sleep(sf::seconds(sleepTime));

        clock.restart();
    }

    //void update() {
    //    deltaTime = clock.restart().asSeconds();
    //}

    float getDeltaTime() const {
        return deltaTime;
    }

};
