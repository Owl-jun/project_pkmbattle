#include "pch.h"
#include "KeyManager.h"


KeyManager& KeyManager::getInstance() {
    static KeyManager instance;
    return instance;
}

void KeyManager::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        const auto* key = event.getIf<sf::Event::KeyPressed>();
        if (key) {
            auto& state = keyStates[key->code];
            if (state == KeyState::None || state == KeyState::Released)
                state = KeyState::Pressed;
        }
    }
    else if (event.is<sf::Event::KeyReleased>()) {
        const auto* key = event.getIf<sf::Event::KeyReleased>();
        if (key) {
            keyStates[key->code] = KeyState::Released;
        }
    }
}

void KeyManager::update() {
    for (auto& [key, state] : keyStates) {
        if (state == KeyState::Pressed)
            state = KeyState::Held;
        else if (state == KeyState::Released)
            state = KeyState::None;
    }
}

// �����ų� ��� ������ ����
bool KeyManager::isKeyPressed(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    if (it == keyStates.end()) return false;
    return it->second == KeyState::Pressed || it->second == KeyState::Held;
}

// �� ���� (�� �����Ӹ� true)
bool KeyManager::isKeyDown(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second == KeyState::Pressed;
}

// �� �� (�� �����Ӹ� true)
bool KeyManager::isKeyUp(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second == KeyState::Released;
}


sf::Vector2f KeyManager::getDirection() const {
    sf::Vector2f dir(0.f, 0.f);
    if (isKeyPressed(sf::Keyboard::Key::Up))    dir.y -= 1.f;
    if (isKeyPressed(sf::Keyboard::Key::Down))  dir.y += 1.f;
    if (isKeyPressed(sf::Keyboard::Key::Left))  dir.x -= 1.f;
    if (isKeyPressed(sf::Keyboard::Key::Right)) dir.x += 1.f;
    return dir;
}

