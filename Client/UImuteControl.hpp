#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "KeyManager.h"
#include "SoundManager.hpp"
#include "ResourceManager.hpp"

class UImuteControl : public BaseUI {
private:
    sf::Texture* muteTex;
    sf::Texture* unmuteTex;
    std::optional<sf::Sprite> icon;
    bool isHovered = false;

public:
    UImuteControl(const sf::Vector2f& pos) {
        muteTex = &ResourceManager::getInstance().getTexture("C:/Source/project_pkmbattle/Client/assets/mute_icon.png");
        unmuteTex = &ResourceManager::getInstance().getTexture("C:/Source/project_pkmbattle/Client/assets/unmute_icon.png");

        icon.emplace(*unmuteTex);
        icon->setTexture(*unmuteTex);
        icon->setPosition(pos);
        icon->setScale({ 0.5f, 1.f });
        icon->setColor(sf::Color(255, 255, 255, 255)); // 초기 불투명
    }

    void setPosition(const sf::Vector2f& pos) {
        if (icon.has_value()) {
            icon->setPosition(pos);
        }
    }


    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        if (!icon.has_value()) return;

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        isHovered = icon->getGlobalBounds().contains(mousePos);

        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse->button == sf::Mouse::Button::Left && isHovered) {
                if (SoundManager::getInstance().isPlaying()) {
                    SoundManager::getInstance().pauseMusic();
                }
                else {
                    SoundManager::getInstance().resumeMusic();
                }
            }
        }
    }

    void update(sf::RenderWindow& window) override {
        if (!icon.has_value()) return;

        if (SoundManager::getInstance().isPlaying()) {
            icon->setTexture(*unmuteTex);
        }
        else {
            icon->setTexture(*muteTex);
        }

        if (isHovered) {
            icon->setColor(sf::Color(255, 255, 255, 180));
        }
        else {
            icon->setColor(sf::Color(255, 255, 255, 255));
        }
    }

    void render(sf::RenderWindow& window) override {
        if (icon.has_value()) {
            window.draw(*icon);
        }
    }
};
