// Player.cpp
#include "pch.h"
#include "Player.h"
#include "KeyManager.h"
#include "ResourceManager.hpp"
#include <cmath>


Player::Player() {
    for (int i = 0; i <= 9; ++i) {
        std::string path = "C:/Source/project_pkmbattle/Client/assets/player0" + std::to_string(i) + ".png";
        sf::Texture& tex = ResourceManager::getInstance().getTexture(path);

        // ���� �� �ִϸ��̼� ����
        if (i <= 2) downFrames.push_back(tex);
        else if (i <= 4) leftFrames.push_back(tex);
        else if (i <= 6) rightFrames.push_back(tex);
        else upFrames.push_back(tex);
    }

    // ���� ��ġ������ (���� ���� ����� ����� ��ġ�� ����)
    tilePos = { 2, 39 };
    targetWorldPos = sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize);

    // ����ǥ (targetWorldPos) �� ĳ���� ����
    if (!downFrames.empty()) {
        sprite.emplace(downFrames[0]);
        sprite->setPosition(targetWorldPos);
        sprite->setScale({ 1.f, 1.f });
    }
}

// ��ǥ ��� �� ���� (2���� ����)
void Player::setCollisionMap(std::vector<std::vector<int>>* map) {
    collisionMap = map;
}

// ���� �� �̳� �浹����(1)�� �ִٸ� false , �ƴ϶�� �����ϼ� ���� true
bool Player::canMoveTo(Direction dir) {
    if (!collisionMap) return true;

    sf::Vector2i nextTile = tilePos;
    switch (dir) {
    case Direction::Up:    nextTile.y -= 1; break;
    case Direction::Down:  nextTile.y += 1; break;
    case Direction::Left:  nextTile.x -= 1; break;
    case Direction::Right: nextTile.x += 1; break;
    default: return false;
    }

    if (nextTile.y < 0 || nextTile.y >= collisionMap->size()) return false;
    if (nextTile.x < 0 || nextTile.x >= (*collisionMap)[0].size()) return false;

    return (*collisionMap)[nextTile.y][nextTile.x] == 0;    
}

// �����õ�
void Player::tryMoveInDirection(Direction dir) {
    if (!canMoveTo(dir)) return;

    currentDirection = dir;
    sf::Vector2i offset{ 0, 0 };

    switch (dir) {
    case Direction::Up:    offset.y = -1; break;
    case Direction::Down:  offset.y = 1;  break;
    case Direction::Left:  offset.x = -1; break;
    case Direction::Right: offset.x = 1;  break;
    default: return;
    }

    tilePos += offset;      // ���� ��ǥ
    targetWorldPos = sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize);  // ����ǥ
    isMoving = true;    // �����̴� ��
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
        sf::Vector2f dir = targetWorldPos - pos;    // ��������ġ - ����ġ �� ���⼳��
        float dist = speed * dt;

        if (std::hypot(dir.x, dir.y) <= dist) {     // �̹� �����ӿ� ��ǥ ��������
            sprite->setPosition(targetWorldPos);
            isMoving = false;

            const auto& keyMgr = KeyManager::getInstance();
            if (lastHeldDirection != Direction::None &&
                keyMgr.isKeyPressed(getSfmlKey(lastHeldDirection)) &&
                canMoveTo(lastHeldDirection)) {
                tryMoveInDirection(lastHeldDirection);
            }
        }
        else {
            sf::Vector2f norm = normalize(dir);     // ���� ���ͷ� ��ȯ
            sprite->move(norm * dist);              // �����Ӹ��� ������ �̵��Ϸ�
        }

        animate(dt);
        return;
    }

    currentFrame = 0;
    updateSpriteTexture();
    // ����ڰ� Ű���带 ������ �����ϸ�
    const auto& keyMgr = KeyManager::getInstance();
    lastHeldDirection = Direction::None;

    // ���⿡ �°� lastHeldDirection ������Ʈ
    if (keyMgr.isKeyPressed(sf::Keyboard::Key::Left))   lastHeldDirection = Direction::Left;
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Right))  lastHeldDirection = Direction::Right;
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Up))     lastHeldDirection = Direction::Up;
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Down))   lastHeldDirection = Direction::Down;

    // ������ ����ٸ�
    if (lastHeldDirection != Direction::None) {
        currentDirection = lastHeldDirection;   // ���� ���� ������Ʈ

        if (canMoveTo(lastHeldDirection)) {     // �̵������� ��ǥ
            tryMoveInDirection(lastHeldDirection);  // �����ӽõ� (isMoving True)
        }
        else {
            // �̵� �Ұ����� ��� ���⸸ ��ȯ
            currentFrame = 0;
            updateSpriteTexture();
        }
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

// ĳ���� �ִϸ��̼�
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
