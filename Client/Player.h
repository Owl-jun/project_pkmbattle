// Player.h
#pragma once
#include <optional>

enum class Direction {
    Down, Left, Right, Up, None
};

class Player {
private:
    std::vector<std::shared_ptr<sf::Texture>> downFrames, leftFrames, rightFrames, upFrames;
    std::optional<sf::Sprite> sprite;

    float speed = 250.f;
    float frameTime = 0.01f;
    float elapsedTime = 0.f;
    int currentFrame = 0;
    float moveCooldown = 0.f;
    Direction currentDirection = Direction::Down;
    Direction lastHeldDirection = Direction::None;

    sf::Vector2i tilePos;
    sf::Vector2f targetWorldPos;
    bool isMoving = false;
    const int tileSize = 60;

public:
    Player();
    Player(const Player& other)
        : downFrames(other.downFrames),
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
            sprite.emplace(other.sprite.value());  
            sprite->setPosition(targetWorldPos);
            sprite->setScale({ 1.f, 1.f });
        }
    }
    Player& operator=(const Player& other) {
        if (this == &other) return *this;

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
            sprite.emplace(other.sprite.value());
            sprite->setPosition(targetWorldPos);
            sprite->setScale({ 1.f, 1.f });
        }
        else {
            sprite.reset();
        }

        return *this;
    }

    void sendDirectionToServer(Direction dir);
    void update(float dt,bool isLocalPlayer);
    void draw(sf::RenderWindow& window);
    void setPosition(const sf::Vector2f& pos);
    void setTargetTilePosition(const sf::Vector2i& pos);
    sf::Vector2f getPosition() const;
    sf::Vector2i getTilePosition() const;
    sf::Vector2f normalize(const sf::Vector2f& v);

private:
    void animate(float dt);
    void updateSpriteTexture();
    std::vector<std::shared_ptr<sf::Texture>>* getCurrentFrameSet();

};