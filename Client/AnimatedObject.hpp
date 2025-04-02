#pragma once
#include "pch.h"
#include "TimeManager.hpp"
#include <cmath>
#include <functional>

class AnimatedObject {
private:
    const sf::Texture* texture = nullptr;
    std::optional<sf::Sprite> sprite;
    sf::Vector2f basePos;
    sf::Vector2f m_Pos;

    // 애니메이션 환경 변수
    float elapsed = 0.f;
    float timer = 0.f;
    float alpha = 255.f;    
    bool started = false;
    bool finished = false;

    // 애니메이션 관련 매개 변수
    int direction = -1;     // 방향
    float speed = 0.f;      // 속도
    float amplitude = 0.f;  // 진폭
    float freq = 0.f;       // 주기
    float delay = 0.f;      // 딜레이타임

public:
    AnimatedObject() = default;
    AnimatedObject(const std::string& fileName, sf::Vector2f pos, float _delay = 0.f)
    {
        texture = &ResourceManager::getInstance().getTextureByName(fileName);
        sprite.emplace(*texture);
        basePos = pos;
        m_Pos = pos;
        sprite->setPosition(pos);
        sprite->setColor(sf::Color(255, 255, 255, alpha));
        delay = _delay;
    }
    AnimatedObject(const std::string& fileName, sf::Vector2f pos, float _speed, float _freq, int _direction = -1, float _amplitude = 0.f)
    {
        texture = &ResourceManager::getInstance().getTextureByName(fileName);
        sprite.emplace(*texture);
        basePos = pos;
        m_Pos = pos;
        sprite->setPosition(pos);
        sprite->setColor(sf::Color(255, 255, 255, alpha));
        speed = _speed;
        freq = _freq;
        direction = _direction;
        amplitude = _amplitude;
    }
    // 객체 별 애니메이션 설정
    void update(std::function<void(AnimatedObject&, float)> func, float dt) {
        func(*this, dt);
    }
    void draw(sf::RenderWindow& window) {
        if (!finished)
            window.draw(*sprite);
    }
    bool isFinished() { return finished; }

    // --------------------------------------------------------------------------
    // 생성자별 사용가능 애니메이션
    // AnimatedObject(const std::string& fileName, sf::Vector2f pos, float _delay)
    void fadein(float dt) {
        alpha = 0.f;
        // 딜레이 처리
        elapsed += dt;
        if (!started) {
            if (elapsed >= delay) {
                started = true;
                elapsed = 0.f;
            }
            else return;
        }
        // 페이드 인 기능
        sprite->setColor(sf::Color(255, 255, 255, alpha));
        timer += dt;
        if (started && (alpha < 255.f)) {
            alpha += dt * speed;
            if (alpha >= 255.f) {
                alpha = 255.f;
                started = false;
                timer = 0.f;
                finished = true;
            }
            sprite->setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
        }
    }

    void fadeout(float dt) {
        // 딜레이 처리
        elapsed += dt;
        if (!started) {
            if (elapsed >= delay) {
                started = true;
                elapsed = 0.f;
            }
            else return;
        }
        // 페이드 아웃 기능
        sprite->setColor(sf::Color(255, 255, 255, alpha));
        timer += dt;
        if (started && (alpha < 255.f)) {
            alpha -= dt * speed;
            if (alpha <= 0.f) {
                alpha = 0.f;
                started = false;
                timer = 0.f;
                finished = true;
            }
            sprite->setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
        }
    }
    // --------------------------------------------------------------------------



    // --------------------------------------------------------------------------
    // AnimatedObject(const std::string& fileName, sf::Vector2f pos, float _speed, float _freq, int _direction = -1, float _amplitude = 0.f)
    // 한쪽방향 무한루프 이동
    void move(float dt,float windowWidth) {
        timer += dt;
        if (speed != 0.f) {
            m_Pos.x += speed * dt * direction;
            if (windowWidth > 0.f && m_Pos.x <= -300) {
                m_Pos.x = windowWidth + 200;
            }
        }
        if (amplitude != 0.f && freq != 0.f) {
            m_Pos.y = basePos.y + std::sin(dt * freq) * amplitude;
        }
        sprite->setPosition(m_Pos);
    }
    // 좌우 방향 변경 이동
    void move(float dt) {
        if (m_Pos.x >= 800.f || m_Pos.x <= 0.f) { direction *= -1; }
        if (m_Pos.y >= 150.f || m_Pos.y <= 0.f) { direction *= -1; }
        if (speed != 0.f) {
            m_Pos.x += speed * dt * direction;
            m_Pos.y += speed * dt * direction;
        }
        sprite->setPosition(m_Pos);
    }
    // 위 아래 이동
    void bounce(float dt,float top, float down) {
        if (m_Pos.y <= top) { direction = 1; }
        else if (m_Pos.y >= down) { direction = -1; }
        m_Pos.y += speed * dt * direction;

        sprite->setPosition(m_Pos);
    }
    // --------------------------------------------------------------------------


    // ---------------------------------------------------------------------
    // 게터, 세터
    void setScale(sf::Vector2f scale) {
        sprite->setScale(scale);
    }

    void setPosition(sf::Vector2f pos) {
        basePos = pos;
        sprite->setPosition(pos);
    }

    void setOriginCenter() {
        auto bounds = sprite->getLocalBounds();
        sprite->setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    }

    sf::Sprite& getSprite() {
        return *sprite;
    }
    // ---------------------------------------------------------------------
};
