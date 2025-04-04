// Player.cpp
#include "pch.h"
#include "Player.hpp"
#include "KeyManager.hpp"
#include "TimeManager.hpp"
#include <cmath>

// -------------------------------------------------------
// loop 3종 세트
void Player::handleInput(const sf::Event& event, sf::RenderWindow& window, bool isLocalPlayer) {
    const auto& keyMgr = KeyManager::getInstance();
    lastHeldDirection = Direction::None;

    if (!isMoving) {
        if (isLocalPlayer) {
            if (keyMgr.isKeyPressed(sf::Keyboard::Key::Left))      lastHeldDirection = Direction::Left;
            else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Right)) lastHeldDirection = Direction::Right;
            else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Up))    lastHeldDirection = Direction::Up;
            else if (keyMgr.isKeyPressed(sf::Keyboard::Key::Down))  lastHeldDirection = Direction::Down;
        }

        if (lastHeldDirection != Direction::None && moveCooldown <= 0.f) {
            currentDirection = lastHeldDirection;

            sendDirectionToServer(currentDirection);
            moveCooldown = 0.025f;
            setCapDir();
            updateSpriteTexture();
        }
    }

}

void Player::update(float dt) {
    if (!sprite.has_value()) return;
    if (isGetCap()) { speed = 450.f; }
    else { speed = 350.f; }
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
        setCapDir();
        animate(dt);
        return;
    }
    else {
        currentFrame = 0;
        if (auto* frames = getCurrentFrameSet(colorMode); frames && !frames->empty()) {
            sprite->setTexture(*(*frames)[0]);
        }
    }
    cap->setPosition({ sprite->getGlobalBounds().getCenter() });
    if (speechTimer > 0.f)
        speechTimer -= dt;
}

void Player::draw(sf::RenderWindow& window) {
    if (sprite.has_value()) {
        window.draw(*sprite);
    }
    if (getCap) {
        window.draw(*cap);
    }

    if (speechTimer > 0.f) {
        // 위치 설정
        sf::FloatRect bounds = speechText.getGlobalBounds();
        sf::Vector2f size = bounds.size;
        sf::Vector2f pos = getPosition(); // 머리 위에 띄우기
        speechBubble.setPosition({ (pos.x + 30.f) - (size.x/2.f) , pos.y - 35.f });
        speechText.setPosition({ speechBubble.getPosition().x + 2.f, speechBubble.getPosition().y - (size.y - 5.f) });

        window.draw(speechBubble);
        window.draw(speechText);
    }
}

// ----------------------------------------------------------
// 이동관련

// 키보드 입력 받고 서버로 패킷 전송
void Player::sendDirectionToServer(Direction dir) {
    std::string directionStr;

    switch (dir) {
    case Direction::Up:    directionStr = "UP"; break;
    case Direction::Down:  directionStr = "DOWN"; break;
    case Direction::Left:  directionStr = "LEFT"; break;
    case Direction::Right: directionStr = "RIGHT"; break;
    default: return;
    }

    if (!isMoving) {
        std::string msg = "MOVE " + directionStr + "\n";
        NetworkManager::getInstance().send(msg);
    }
}

void Player::move(const sf::Vector2i& pos, Direction dir) {
    if (isMoving) return;
    if (!sprite.has_value()) return;
    tilePos = pos;
    targetWorldPos = static_cast<sf::Vector2f>(pos) * static_cast<float>(tileSize);

    sf::Vector2f currentPos = sprite->getPosition();
    if (targetWorldPos == currentPos) return;

    isMoving = true;
    currentDirection = dir;
}

// ----------------------------------------------------------
// 애니메이션 관련
void Player::animate(float dt) {
    elapsedTime += dt;
    auto* frames = getCurrentFrameSet(colorMode);
    if (!frames || frames->empty()) return;
    if (currentFrame >= frames->size()) currentFrame = 0;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.f;
        currentFrame = (currentFrame + 1) % frames->size();
        sprite->setTexture(*(*frames)[currentFrame]);
    }
    
}

void Player::updateSpriteTexture() {
    auto* frames = getCurrentFrameSet(colorMode);
    if (!frames || frames->empty()) return;

    if (currentFrame >= frames->size()) {
        currentFrame = 0;  // 방어 코드
    }

    sprite->setTexture(*(*frames)[currentFrame]);
}

std::vector<std::shared_ptr<sf::Texture>>* Player::getCurrentFrameSet(int colorMode) {
    // colorMode 0 : default || 1 : Blue || 2 : Green || 3 : pink
    if (colorMode == 0) {
        switch (currentDirection) {
        case Direction::Down:  return &downFrames;
        case Direction::Left:  return &leftFrames;
        case Direction::Right: return &rightFrames;
        case Direction::Up:    return &upFrames;
        default:               return nullptr;
        }
    }
    else if (colorMode == 1) {
        switch (currentDirection) {
        case Direction::Down:  return &downFrames2;
        case Direction::Left:  return &leftFrames2;
        case Direction::Right: return &rightFrames2;
        case Direction::Up:    return &upFrames2;
        default:               return nullptr;
        }
    }
    else if (colorMode == 2) {
        switch (currentDirection) {
        case Direction::Down:  return &downFrames3;
        case Direction::Left:  return &leftFrames3;
        case Direction::Right: return &rightFrames3;
        case Direction::Up:    return &upFrames3;
        default:               return nullptr;
        }
    }
    else if (colorMode == 3) {
        switch (currentDirection) {
        case Direction::Down:  return &downFrames4;
        case Direction::Left:  return &leftFrames4;
        case Direction::Right: return &rightFrames4;
        case Direction::Up:    return &upFrames4;
        default:               return nullptr;
        }
    }
    
}
// ----------------------------------------------------------

// ----------------------------------------------------------
// 상호작용 관련
// 현재 바라보는 방향으로 한 칸 앞 위치 계산
sf::Vector2i Player::getTileInFront() const {
    switch (currentDirection) {
    case Direction::Up: return { tilePos.x, tilePos.y - 1 };
    case Direction::Down: return { tilePos.x, tilePos.y + 1 };
    case Direction::Left: return { tilePos.x - 1, tilePos.y };
    case Direction::Right: return { tilePos.x + 1, tilePos.y };
    default: return tilePos;
    }
}
// ----------------------------------------------------------

// ----------------------------------------------------------
// 채팅관련
void Player::showSpeechBubble(const std::string& msg) {
    sf::String unicodeMsg = sf::String::fromUtf8(msg.begin(), msg.end());
    sf::String wrapped = wrapText(unicodeMsg, 200, font, 18);
    speechText.setFont(font);
    speechText.setString(unicodeMsg);
    speechText.setCharacterSize(18);
    speechText.setFillColor(sf::Color::Black);

    // 말풍선 배경 설정
    sf::FloatRect bounds = speechText.getGlobalBounds();
    sf::Vector2f size = bounds.size;

    speechBubble.setSize({ size.x + 20.f, size.y + 10.f });
    speechBubble.setFillColor(sf::Color::White);
    speechBubble.setOutlineColor(sf::Color::Black);
    speechBubble.setOutlineThickness(2.f);

    speechTimer = speechDuration;
}

sf::String Player::wrapText(const sf::String& str, unsigned int maxWidth, const sf::Font& font, unsigned int characterSize) {
    sf::Text tempText(font,"", characterSize);
    sf::String wrapped;
    sf::String currentLine;

    for (std::size_t i = 0; i < str.getSize(); ++i) {
        currentLine += str[i];
        tempText.setString(currentLine);
        if (tempText.getLocalBounds().size.x > maxWidth) {
            wrapped += '\n';
            currentLine = str[i];
        }
        else {
            wrapped += str[i];
        }
    }

    return wrapped;
}
// ----------------------------------------------------------



// ----------------------------------------------------------
// 게터 세터

void Player::setGetCap(bool isGet)
{
    getCap = isGet;
}

void Player::setCapDir()
{
    switch (currentDirection) {
    case Direction::Down:  cap->setTexture(*CAP[0]); break;
    case Direction::Left:  cap->setTexture(*CAP[2]); break;
    case Direction::Right: cap->setTexture(*CAP[3]); break;
    case Direction::Up:    cap->setTexture(*CAP[1]); break;
    default:               break;
    }
}
void Player::setColor(int num) {
    colorMode = num;
}

void Player::setTile(sf::Vector2i& pos) {
    tilePos = pos;
}

void Player::setPosition(const sf::Vector2f& pos) {
    if (sprite.has_value())
        sprite->setPosition(pos);
}

void Player::setCurDir(std::string d) {
    if (!d.empty()) {
        if (d == "UP") { currentDirection = Direction::Up; }
        else if (d == "DOWN") { currentDirection = Direction::Down; }
        else if (d == "RIGHT") { currentDirection = Direction::Right; }
        else if (d == "LEFT") { currentDirection = Direction::Left; }
    }
    updateSpriteTexture();
}

sf::Vector2i Player::getTilePosition() const {
    return tilePos;
}

bool Player::isGetCap()
{
    return getCap;
}

sf::Vector2f Player::getPosition() const {
    return sprite.has_value() ? sprite->getPosition() : sf::Vector2f{ 0.f, 0.f };
}
// ----------------------------------------------------------

// ----------------------------------------------------------
// 유틸 메서드
sf::Vector2f Player::normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return len != 0.f ? sf::Vector2f(v.x / len, v.y / len) : sf::Vector2f(0.f, 0.f);
}
// ----------------------------------------------------------
