#include "pch.h"
#include "KeyManager.hpp"


KeyManager& KeyManager::getInstance() {
    static KeyManager instance;
    return instance;
}

void KeyManager::handleInput(const sf::Event& event) {
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

// 눌렀거나 계속 누르고 있음
bool KeyManager::isKeyPressed(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    if (it == keyStates.end()) return false;
    return it->second == KeyState::Pressed || it->second == KeyState::Held;
}

// 막 눌림 (한 프레임만 true)
bool KeyManager::isKeyDown(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second == KeyState::Pressed;
}

// 막 뗌 (한 프레임만 true)
bool KeyManager::isKeyUp(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second == KeyState::Released;
}


