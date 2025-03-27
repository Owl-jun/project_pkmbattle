#pragma once

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

    float speed = 100.f;
    float frameTime = 0.1f;
    float elapsedTime = 0.f;
    int currentFrame = 0;

    Direction currentDirection = Direction::Down;

public:
    Player();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;

private:
    void animate(float dt);
    void updateSpriteTexture();
    std::vector<sf::Texture>* getCurrentFrameSet();
};
