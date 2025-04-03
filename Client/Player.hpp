﻿#pragma once
#include "pch.h"
#include <optional>

enum class Direction {
    Down, Left, Right, Up, None
};

class Player {
private:
    std::vector<std::shared_ptr<sf::Texture>> downFrames, leftFrames, rightFrames, upFrames;
    std::optional<sf::Sprite> sprite;
    
    float speed = 350.f;
    float frameTime = 0.01f;
    float elapsedTime = 0.f;
    int currentFrame = 0;
    float moveCooldown = 0.f;

    // 현재 방향
    Direction currentDirection = Direction::Down;
    Direction lastHeldDirection = Direction::None;

    // 플레이어 정보
    sf::Vector2i tilePos;           // 현재 타일기준 좌표
    std::string name;
    int win;
    int lose;
    int level;
    float exp;
    sf::Vector2f targetWorldPos;    // 이동할 실 좌표

    bool isMoving = false;
    const int tileSize = 60;

    // 채팅 관련
    sf::Font font = ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF");
    sf::RectangleShape speechBubble;
    sf::Text speechText;
    float speechTimer = 0.f;
    const float speechDuration = 3.0f; // 3초간 유지

public:
    // 3종세트
    void handleInput(const sf::Event& event, sf::RenderWindow& window, bool isLocalPlayer);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    // 채팅관련
    void showSpeechBubble(const std::string& msg);
    sf::String wrapText(const sf::String& str, unsigned int maxWidth, const sf::Font& font, unsigned int characterSize);
    
    // 이동관련
    void sendDirectionToServer(Direction dir);
    void move(const sf::Vector2i& pos, Direction dir);

    sf::Vector2i getTileInFront() const;
    void setCurDir(std::string d);


    // 게터 세터
    void setPosition(const sf::Vector2f& pos);
    void setTile(sf::Vector2i& pos);
    sf::Vector2f getPosition() const;
    sf::Vector2i getTilePosition() const;

private:
    void animate(float dt);
    void updateSpriteTexture();
    std::vector<std::shared_ptr<sf::Texture>>* getCurrentFrameSet();
    sf::Vector2f normalize(const sf::Vector2f& v);

public:
    // ---------------------------------------------------------------------
    // 생성자 종합세트
    Player() : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF")) , speechText(font, "", 24)  {};
    Player(std::string _name , int x, int y, int _win , int _lose , int _level, float _exp ) 
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , speechText(font, "", 24)
        , name(_name)
        , win(_win)
        , lose(_lose)
        , level(_level)
        , exp(_exp)
    {
        for (int i = 0; i <= 9; ++i) {
            std::string path = "player0" + std::to_string(i) + ".png";
            std::shared_ptr<sf::Texture> tex = std::make_shared<sf::Texture>(
                ResourceManager::getInstance().getTextureByName(path)
            );

            if (i <= 2) downFrames.push_back(tex);
            else if (i <= 4) leftFrames.push_back(tex);
            else if (i <= 6) rightFrames.push_back(tex);
            else upFrames.push_back(tex);
        }

        tilePos = { x, y };
        targetWorldPos = static_cast<sf::Vector2f>(tilePos) * static_cast<float>(tileSize);

        if (!downFrames.empty() && downFrames[0]->getSize().x > 0) {
            sprite.emplace(*downFrames[0]);
            sprite->setPosition(targetWorldPos);
            sprite->setScale({ 1.f, 1.f });
        }
        currentDirection = Direction::Down;
    };
    Player(const Player& other)
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF")),
        speechText(font, "", 24),
        name(other.name),
        win(other.win),
        lose(other.lose),
        level(other.level),
        exp(other.exp),
        downFrames(other.downFrames),
        leftFrames(other.leftFrames),
        rightFrames(other.rightFrames),
        upFrames(other.upFrames),
        currentDirection(other.currentDirection),
        lastHeldDirection(other.lastHeldDirection),
        tilePos(other.tilePos),
        targetWorldPos(other.targetWorldPos),
        isMoving(other.isMoving),
        speed(other.speed),
        frameTime(other.frameTime),
        elapsedTime(other.elapsedTime),
        currentFrame(other.currentFrame)
    {

        if (other.sprite.has_value()) {
            sprite.emplace(*other.sprite);
            sprite->setPosition(targetWorldPos);
            sprite->setScale({ 1.f, 1.f });
        }
    }
    Player& operator=(const Player& other) {
        if (this == &other) return *this;
        font = other.font;
        name = other.name;
        win = other.win;
        lose = other.lose;
        level = other.level;
        exp = other.exp;
        downFrames = other.downFrames;
        leftFrames = other.leftFrames;
        rightFrames = other.rightFrames;
        upFrames = other.upFrames;

        currentDirection = other.currentDirection;
        lastHeldDirection = other.lastHeldDirection;
        tilePos = other.tilePos;
        targetWorldPos = other.targetWorldPos;
        isMoving = other.isMoving;
        speed = other.speed;
        frameTime = other.frameTime;
        elapsedTime = other.elapsedTime;
        currentFrame = other.currentFrame;

        if (other.sprite.has_value()) {
            sprite.emplace(*other.sprite);
            sprite->setPosition(targetWorldPos);
            sprite->setScale({ 1.f, 1.f });
        }
        else {
            sprite.reset();
        }

        return *this;
    }
    // ---------------------------------------------------------------------

};