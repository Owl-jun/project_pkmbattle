#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

enum class Direction {
    Down,
    Left,
    Right,
    Up,
    None
};

class Player {
private:
    // 방향별 애니메이션 프레임
    std::vector<sf::Texture> downFrames;
    std::vector<sf::Texture> leftFrames;
    std::vector<sf::Texture> rightFrames;
    std::vector<sf::Texture> upFrames;

    std::optional<sf::Sprite> sprite;

    float speed = 200.f;
    float frameTime = 0.1f;
    float elapsedTime = 0.f;
    int currentFrame = 0;

    Direction currentDirection = Direction::Down;
    Direction lastHeldDirection = Direction::None;

    // 타일 이동 관련
    sf::Vector2i tilePos;
    sf::Vector2f targetWorldPos;
    bool isMoving = false;
    const int tileSize = 60;

public:
    Player();
    void tryMoveInDirection(Direction dir);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    sf::Keyboard::Key getSfmlKey(Direction dir) const;
private:
    void animate(float dt);
    void updateSpriteTexture();
    std::vector<sf::Texture>* getCurrentFrameSet();
    sf::Vector2f normalize(const sf::Vector2f& v);
};
