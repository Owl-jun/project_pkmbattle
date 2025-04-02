#pragma once
#include "pch.h"

class TimeManager {
private:
    sf::Clock clock;
    float deltaTime = 0.f;
    //const float targetFrameTime = 1.f / 30.f; // �⺻ FRAME
     // �⺻ FRAME���� ���� �����ϱ� ���� const�� ����
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
    // FPS���� �����ϱ� ���� �ʿ��� �޼���(�����߰�)
    /*----------------------------------------------------------------------*/
    void setFPS(int newFps) {
        if (newFps == 10 || newFps == 20 || newFps == 30) {  // FPS�� 10, 20, 30�� �����ϵ��� ����
            fps = newFps;
            targetFrameTime = 1.f / static_cast<float>(fps);  // ���ο� FPS�� �´� targetFrameTime ���
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

    //void update() {
    //    deltaTime = clock.restart().asSeconds();
    //}

    float getDeltaTime() const {
        return deltaTime;
    }

};
