#include "pch.h"
#include "Player.h"
#include "KeyManager.h"

Player::Player() {
    for (int i = 0; i <= 9; ++i) {
        sf::Texture tex;
        std::string path = "C:/Source/project_pkmbattle/Client/assets/player0" + std::to_string(i) + ".png";
        if (!tex.loadFromFile(path)) {
            std::cerr << "플레이어 이미지 로딩 실패: " << path << "\n";
        }
        
        if (i <= 2) downFrames.push_back(tex);       // 00~02: 아래 걷기
        else if (i <= 4) leftFrames.push_back(tex);  // 03~04: 왼쪽 걷기
        else if (i <= 6) rightFrames.push_back(tex); // 05~06: 오른쪽 걷기
        else upFrames.push_back(tex);                // 07~09: 위쪽 걷기
    }

    if (!downFrames.empty()) {
        sprite.emplace(downFrames[0]);
        sprite->setPosition({ 400.f, 300.f });
        sprite->setScale({ 1.f, 1.f });
    }
}

void Player::update(float dt) {
    const auto& keyMgr = KeyManager::getInstance();
    Direction newDir = Direction::None;

    if (keyMgr.isKeyPressed(sf::Keyboard::Key::Left)) {
        sprite->move({ -speed * dt, 0.f });
        newDir = Direction::Left;
    }
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Right)) {
        sprite->move({ speed * dt, 0.f });
        newDir = Direction::Right;
    }
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Up)) {
        sprite->move({ 0.f, -speed * dt });
        newDir = Direction::Up;
    }
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Down)) {
        sprite->move({ 0.f, speed * dt });
        newDir = Direction::Down;
    }

    if (newDir != Direction::None) {
        if (currentDirection != newDir) {
            currentDirection = newDir;
            currentFrame = 0;
            elapsedTime = 0.f;
        }
        animate(dt);
    }
    else {
        // 정지 시 첫 프레임 고정
        currentFrame = 0;
        updateSpriteTexture();
    }
}

void Player::animate(float dt) {
    elapsedTime += dt;
    auto* frames = getCurrentFrameSet();
    if (!frames || frames->empty()) return;

    if (elapsedTime >= frameTime) {
        elapsedTime = 0.f;
        currentFrame = (currentFrame + 1) % frames->size();
        sprite->setTexture((*frames)[currentFrame]);
    }
}

void Player::updateSpriteTexture() {
    auto* frames = getCurrentFrameSet();
    if (!frames || frames->empty()) return;
    sprite->setTexture((*frames)[currentFrame]);
}

std::vector<sf::Texture>* Player::getCurrentFrameSet() {
    switch (currentDirection) {
    case Direction::Down:  return &downFrames;
    case Direction::Left:  return &leftFrames;
    case Direction::Right: return &rightFrames;
    case Direction::Up:    return &upFrames;
    default:               return nullptr;
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (sprite.has_value())
        window.draw(*sprite);
}

void Player::setPosition(const sf::Vector2f& pos) {
    if (sprite.has_value())
        sprite->setPosition(pos);
}

sf::Vector2f Player::getPosition() const {
    return sprite.has_value() ? sprite->getPosition() : sf::Vector2f{ 0.f, 0.f };
}
