#include "pch.h"
#include "Player.h"
#include "KeyManager.h"

Player::Player() {
    for (int i = 0; i < 10; ++i) {
        sf::Texture tex;
        std::string path = "C:/Source/project_pkmbattle/Client/assets/player0" + std::to_string(i) + ".png";
        if (!tex.loadFromFile(path)) {
            std::cerr << "플레이어 이미지 로딩 실패: " << path << "\n";
        }
        textures.push_back(tex);
    }
    
    sprite.emplace(textures[0]);
    sprite->setPosition({ 400.f, 300.f });
    sprite->setScale({ 1.f, 1.f });
}

void Player::update(float dt) {
    const auto& keyMgr = KeyManager::getInstance();

    bool moved = false;

    if (keyMgr.isKeyPressed(sf::Keyboard::Key::Left)) {
        moveLeft(dt);
        moved = true;
    }
    else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Right)) {
        moveRight(dt);
        moved = true;
    }

    if (moved) animate(dt);
    else {
        // 정지 시 첫 프레임으로 고정
        currentFrame = 0;
        sprite->setTexture(textures[currentFrame]);
    }
}

void Player::animate(float dt) {
    elapsedTime += dt;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.f;
        currentFrame = (currentFrame + 1) % textures.size();
        sprite->setTexture(textures[currentFrame]);
    }
}

void Player::moveLeft(float dt) {
    sprite->move({ -speed * dt, 0.f });
    if (facingRight) {
        sprite->scale({ -1.f, 1.f });
        facingRight = false;
    }
}

void Player::moveRight(float dt) {
    sprite->move({ speed * dt, 0.f });
    if (!facingRight) {
        sprite->scale({ -1.f, 1.f });
        facingRight = true;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}
