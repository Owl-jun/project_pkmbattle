#include "pch.h"
#include "Player.h"
#include "KeyManager.h"
#include <cmath>
#include <iostream>

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

    tilePos = { 5, 5 };
    targetWorldPos = sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize);

    if (!downFrames.empty()) {
        sprite.emplace(downFrames[0]);
        sprite->setPosition(targetWorldPos);
        sprite->setScale({ 1.f, 1.f });
    }
}

void Player::tryMoveInDirection(Direction dir) {
    currentDirection = dir;
    sf::Vector2i offset{ 0, 0 };

    switch (dir) {
    case Direction::Up:    offset.y = -1; break;
    case Direction::Down:  offset.y = 1;  break;
    case Direction::Left:  offset.x = -1; break;
    case Direction::Right: offset.x = 1;  break;
    default: return;
    }

    tilePos += offset;
    targetWorldPos = sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize);
    isMoving = true;
    currentFrame = 0;
    elapsedTime = 0.f;
}

sf::Keyboard::Key Player::getSfmlKey(Direction dir) const {
    switch (dir) {
    case Direction::Left:  return sf::Keyboard::Key::Left;
    case Direction::Right: return sf::Keyboard::Key::Right;
    case Direction::Up:    return sf::Keyboard::Key::Up;
    case Direction::Down:  return sf::Keyboard::Key::Down;
    default:               return sf::Keyboard::Key::Unknown;
    }
}

void Player::update(float dt) {
    if (!sprite.has_value()) return;

    if (isMoving) {
        sf::Vector2f pos = sprite->getPosition();
        sf::Vector2f dir = targetWorldPos - pos;
        float dist = speed * dt;

        if (std::hypot(dir.x, dir.y) <= dist) {
            sprite->setPosition(targetWorldPos);
            isMoving = false;

            const auto& keyMgr = KeyManager::getInstance();
            if (lastHeldDirection != Direction::None && keyMgr.isKeyPressed(getSfmlKey(lastHeldDirection))) {
                tryMoveInDirection(lastHeldDirection);
            }
        }

        else {
            sf::Vector2f norm = normalize(dir);
            sprite->move(norm * dist);
        }

        animate(dt);
        return;
    }

    const auto& keyMgr = KeyManager::getInstance();
    lastHeldDirection = Direction::None;

    if (keyMgr.isKeyPressed(sf::Keyboard::Key::Left))   lastHeldDirection = Direction::Left;
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Right))  lastHeldDirection = Direction::Right;
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Up))     lastHeldDirection = Direction::Up;
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Down))   lastHeldDirection = Direction::Down;
    else lastHeldDirection = Direction::None;

    if (lastHeldDirection != Direction::None) {
        currentDirection = lastHeldDirection;
        sf::Vector2i offset{ 0, 0 };

        switch (lastHeldDirection) {
        case Direction::Up:    offset.y = -1; break;
        case Direction::Down:  offset.y = 1; break;
        case Direction::Left:  offset.x = -1; break;
        case Direction::Right: offset.x = 1; break;
        default: break;
        }

        tilePos += offset;
        targetWorldPos = sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize);
        isMoving = true;
        currentFrame = 0;
        elapsedTime = 0.f;
    }
    else {
        currentFrame = 0;
        updateSpriteTexture();
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

sf::Vector2f Player::normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return len != 0.f ? sf::Vector2f(v.x / len, v.y / len) : sf::Vector2f(0.f, 0.f);
}
