#pragma once
#include "pch.h"

class TimeManager {
private:
    sf::Clock clock;
    float deltaTime = 0.f;
     // 기본 FRAME에서 값을 변경하기 위해 const를 제거
    /*----------------------------------------------------------------------*/
    float targetFrameTime = 1.f / 30.f;
    int fps = 30;
    /*----------------------------------------------------------------------*/
    TimeManager() = default;

public:
    static TimeManager& getInstance() {
        static TimeManager instance;
        return instance;
    }
    // FPS값을 조정하기 위해 필요한 메서드(새로추가)
    /*----------------------------------------------------------------------*/
    void setFPS(int newFps) {
        if (newFps == 10 || newFps == 20 || newFps == 30) {  // FPS가 10, 20, 30만 가능하도록 제한
            fps = newFps;
            targetFrameTime = 1.f / static_cast<float>(fps);  // 새로운 FPS에 맞는 targetFrameTime 계산
        }
    }
    /*----------------------------------------------------------------------*/
    void tick() {
        float frameStart = clock.getElapsedTime().asSeconds();
        deltaTime = targetFrameTime;

        float elapsed = clock.getElapsedTime().asSeconds() - frameStart;
        float sleepTime = targetFrameTime - elapsed;
        if (sleepTime > 0.f)
            sf::sleep(sf::seconds(sleepTime));

        clock.restart();
    }

    float getDeltaTime() const {
        return deltaTime;
    }

};
