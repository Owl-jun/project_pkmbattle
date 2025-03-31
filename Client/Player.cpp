// Player.cpp
#include "pch.h"
#include "Player.h"
#include "KeyManager.h"
#include "ResourceManager.hpp"
#include "NetworkManager.hpp"
#include <cmath>

Player::Player() {
    for (int i = 0; i <= 9; ++i) {
        std::string path = "C:/Source/project_pkmbattle/Client/assets/player0" + std::to_string(i) + ".png";
        std::shared_ptr<sf::Texture> tex = std::make_shared<sf::Texture>(
            ResourceManager::getInstance().getTexture(path)
        );


        if (i <= 2) downFrames.push_back(tex);
        else if (i <= 4) leftFrames.push_back(tex);
        else if (i <= 6) rightFrames.push_back(tex);
        else upFrames.push_back(tex);
    }

    tilePos = { 2, 39 };
    targetWorldPos = static_cast<sf::Vector2f>(tilePos) * static_cast<float>(tileSize);

    if (!downFrames.empty()) {
        sprite.emplace(*downFrames[0]);
        sprite->setPosition(targetWorldPos);
        sprite->setScale({ 1.f, 1.f });
    }
}

void Player::setTile(sf::Vector2i& pos) {
    tilePos = pos;
}

void Player::setPosition(const sf::Vector2f& pos) {
    if (sprite.has_value())
        sprite->setPosition(pos);
}

void Player::setTargetTilePosition(const sf::Vector2i& pos) {
    tilePos = pos;
    targetWorldPos = static_cast<sf::Vector2f>(pos) * static_cast<float>(tileSize);

    if (!sprite.has_value()) return;

    sf::Vector2f currentPos = sprite->getPosition();
    if (targetWorldPos == currentPos) return;

    isMoving = true;

    sf::Vector2f dir = targetWorldPos - currentPos;
    if (std::abs(dir.x) > std::abs(dir.y))
        currentDirection = (dir.x > 0) ? Direction::Right : Direction::Left;
    else
        currentDirection = (dir.y > 0) ? Direction::Down : Direction::Up;

    // 디버깅 로그 출력 
    std::cout << "[Client] 이동 명령 수신: (" << pos.x << ", " << pos.y << ")\n";

}

sf::Vector2i Player::getTilePosition() const {
    return tilePos;
}

sf::Vector2f Player::getPosition() const {
    return sprite.has_value() ? sprite->getPosition() : sf::Vector2f{ 0.f, 0.f };
}

void Player::update(float dt, bool isLocalPlayer) {
    if (!sprite.has_value()) return;

    moveCooldown -= dt;

    if (isMoving) {
        sf::Vector2f pos = sprite->getPosition();
        sf::Vector2f dir = targetWorldPos - pos;
        float dist = speed * dt;

        if (std::hypot(dir.x, dir.y) <= dist) {
            sprite->setPosition(targetWorldPos);
            isMoving = false;
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

    if (!isMoving) {
        if (keyMgr.isKeyPressed(sf::Keyboard::Key::Left))      lastHeldDirection = Direction::Left;
        else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Right)) lastHeldDirection = Direction::Right;
        else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Up))    lastHeldDirection = Direction::Up;
        else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Down))  lastHeldDirection = Direction::Down;

        if (lastHeldDirection != Direction::None && moveCooldown <= 0.f) {
            if (isLocalPlayer)
            {   
                // 현재 캐릭터가 움직이지않고 방향만 바뀔때 본인 캐릭터만 바뀌는 현상
                // 의심지점 , 해결방법 ? 서버에서 방향도 전송?
                currentDirection = lastHeldDirection;
                sendDirectionToServer(currentDirection);
                moveCooldown = 0.15f;
            }
            animate(dt);
        }
    }

    currentFrame = 0;
    updateSpriteTexture();
}

std::vector<std::shared_ptr<sf::Texture>>* Player::getCurrentFrameSet() {
    switch (currentDirection) {
    case Direction::Down:  return &downFrames;
    case Direction::Left:  return &leftFrames;
    case Direction::Right: return &rightFrames;
    case Direction::Up:    return &upFrames;
    default:               return nullptr;
    }
}

void Player::sendDirectionToServer(Direction dir) {
    std::string directionStr;

    switch (dir) {
    case Direction::Up:    directionStr = "UP"; break;
    case Direction::Down:  directionStr = "DOWN"; break;
    case Direction::Left:  directionStr = "LEFT"; break;
    case Direction::Right: directionStr = "RIGHT"; break;
    default: return;
    }

    std::string msg = "MOVE " + directionStr + "\n";
    NetworkManager::getInstance().send(msg);
}

void Player::draw(sf::RenderWindow& window) {
    if (sprite.has_value())
        window.draw(*sprite);
}

void Player::animate(float dt) {
    elapsedTime += dt;
    auto* frames = getCurrentFrameSet();
    if (!frames || frames->empty()) return;

    if (elapsedTime >= frameTime) {
        elapsedTime = 0.f;
        currentFrame = (currentFrame + 1) % frames->size();
        sprite->setTexture(*(*frames)[currentFrame]);
    }
}

void Player::updateSpriteTexture() {
    auto* frames = getCurrentFrameSet();
    if (!frames || frames->empty()) return;
    sprite->setTexture(*(*frames)[currentFrame]);
}

sf::Vector2f Player::normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return len != 0.f ? sf::Vector2f(v.x / len, v.y / len) : sf::Vector2f(0.f, 0.f);
}