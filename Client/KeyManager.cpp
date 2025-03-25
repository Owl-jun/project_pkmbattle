#include "pch.h"
#include "KeyManager.h"

void KeyManager::handleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::KeyPressed>()) {
        const auto* key = event.getIf<sf::Event::KeyPressed>();
        if (key) keyStates[key->code] = true;
    }
    else if (event.is<sf::Event::KeyReleased>()) {
        const auto* key = event.getIf<sf::Event::KeyReleased>();
        if (key) keyStates[key->code] = false;
    }
}

bool KeyManager::isKeyPressed(sf::Keyboard::Key key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

sf::Vector2f KeyManager::getDirection() const
{
    sf::Vector2f dir(0.f, 0.f);
    if (isKeyPressed(sf::Keyboard::Key::W)) dir.y -= 1.f;
    if (isKeyPressed(sf::Keyboard::Key::S)) dir.y += 1.f;
    if (isKeyPressed(sf::Keyboard::Key::A)) dir.x -= 1.f;
    if (isKeyPressed(sf::Keyboard::Key::D)) dir.x += 1.f;
    return dir;
}
