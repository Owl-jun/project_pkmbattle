#pragma once
#include <SFML/Window.hpp>
#include <unordered_map>

enum class KeyState {
    None,
    Pressed,
    Held,
    Released
};

class KeyManager {
private:
    std::unordered_map<sf::Keyboard::Key, KeyState> keyStates;

    KeyManager() = default;
    KeyManager(const KeyManager&) = delete;
    KeyManager& operator=(const KeyManager&) = delete;

public:
    static KeyManager& getInstance();

    void handleInput(const sf::Event& event);
    void update();

    bool isKeyPressed(sf::Keyboard::Key key) const;   
    bool isKeyDown(sf::Keyboard::Key key) const;     
    bool isKeyUp(sf::Keyboard::Key key) const;       
};
