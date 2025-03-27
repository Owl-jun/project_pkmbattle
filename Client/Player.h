#pragma once

class Player {
private:
    std::vector<sf::Texture> textures;
    std::optional<sf::Sprite> sprite;

    float speed = 100.f;
    bool facingRight = true;

    float frameTime = 0.1f; // 프레임당 지속 시간 (초)
    float elapsedTime = 0.f;
    int currentFrame = 0;

public:
    Player();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;

private:
    void animate(float dt);
    void moveLeft(float dt);
    void moveRight(float dt);
};
