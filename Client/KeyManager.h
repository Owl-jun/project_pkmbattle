#pragma once
class KeyManager {
private:
    std::unordered_map<sf::Keyboard::Key, bool> keyStates;

    KeyManager() = default;
    KeyManager(const KeyManager&) = delete;
    KeyManager& operator=(const KeyManager&) = delete;
public:
    static KeyManager& getInstance()
    {
        static KeyManager instance;
        return instance;
    }
    
    void handleEvent(const sf::Event& event);
    bool isKeyPressed(sf::Keyboard::Key key) const;
    sf::Vector2f getDirection() const;
};

