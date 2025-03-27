// Player.h
#pragma once
#include <optional>

enum class Direction {
    Down, Left, Right, Up, None
};

class Player {
private:
    std::vector<sf::Texture> downFrames, leftFrames, rightFrames, upFrames;
    std::optional<sf::Sprite> sprite;

    float speed = 250.f;
    float frameTime = 0.01f;
    float elapsedTime = 0.f;
    int currentFrame = 0;

    Direction currentDirection = Direction::Down;
    Direction lastHeldDirection = Direction::None;

    sf::Vector2i tilePos;
    sf::Vector2f targetWorldPos;
    bool isMoving = false;
    const int tileSize = 60;

    std::vector<std::vector<int>>* collisionMap = nullptr;

public:
    Player();
    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    void setCollisionMap(std::vector<std::vector<int>>* map);
    sf::Keyboard::Key getSfmlKey(Direction dir) const;
    sf::Vector2f normalize(const sf::Vector2f& v);

private:
    void animate(float dt);
    void updateSpriteTexture();
    std::vector<sf::Texture>* getCurrentFrameSet();
    void tryMoveInDirection(Direction dir);
    bool canMoveTo(Direction dir);
};