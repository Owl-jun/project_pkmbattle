#pragma once
#include "pch.h"
#include "UIManager.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include <unordered_set>

class CharacterSelectOverlay {
private:
    sf::RectangleShape background;
    sf::Text title;
    sf::Font& font;

    std::vector<std::string> texturePaths = {
        "Poke1.png", "Poke2.png","Poke3.png","Poke4.png","Poke5.png",
        "Poke6.png","Poke7.png","Poke8.png","Poke9.png","Poke10.png",
    };

    std::vector<sf::Sprite> sprites;
    std::vector<sf::RectangleShape> borders;
    std::unordered_set<int> selectedIndices;

    std::function<void(const std::vector<int>&)> onComplete;
    bool visible = false;

public:
    CharacterSelectOverlay(const sf::Vector2f& size, sf::Font& sharedFont, std::function<void(const std::vector<int>&)> onDone)
        : font(sharedFont), onComplete(onDone), title(font, { "공백" }, 50)
    {
        background.setSize(size);
        background.setFillColor(sf::Color(0, 0, 0, 200));
        background.setPosition({ 0.f, 0.f });

        title.setFont(font);
        title.setString(L"포켓몬 3마리를 선택하세요!");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color::White);
        title.setPosition({ 180.f, 30.f });

        for (int i = 0; i < texturePaths.size(); ++i) {
            auto& tex = ResourceManager::getInstance().getTexture("C:/Source/project_pkmbattle/Client/assets/" + texturePaths[i]);
            sf::Sprite sprite(tex);
            sprite.setScale({ 0.6f, 0.6f });
            sprite.setPosition({ 80.f + (i % 5) * 130.f, 100.f + (i / 5) * 130.f });
            sprites.push_back(sprite);

            sf::RectangleShape border({ 64.f * 2, 64.f * 2 });
            border.setPosition(sprite.getPosition());
            border.setFillColor(sf::Color::Transparent);
            border.setOutlineThickness(4.f);
            border.setOutlineColor(sf::Color::Transparent);
            borders.push_back(border);
        }
    }

    void toggle() { visible = !visible; }
    void show() { visible = true; }
    void hide() { visible = false; }
    bool isVisible() const { return visible; }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (!visible) return;

        if (event.is<sf::Event::MouseButtonPressed>()) {
            auto mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse->button == sf::Mouse::Button::Left) {  // ✅ 고침
                sf::Vector2f mousePos = window.mapPixelToCoords({ mouse->position.x, mouse->position.y });
                for (int i = 0; i < sprites.size(); ++i) {
                    if (sprites[i].getGlobalBounds().contains(mousePos)) {
                        if (selectedIndices.count(i)) {
                            selectedIndices.erase(i);
                            borders[i].setOutlineColor(sf::Color::Transparent);
                        }
                        else if (selectedIndices.size() < 3) {
                            selectedIndices.insert(i);
                            borders[i].setOutlineColor(sf::Color::Green);
                        }
                        break;
                    }
                }
            }
        }

        if (selectedIndices.size() == 3) {
            std::vector<int> result(selectedIndices.begin(), selectedIndices.end());
            if (onComplete) onComplete(result);
            selectedIndices.clear();
            for (auto& b : borders) b.setOutlineColor(sf::Color::Transparent);
            hide();
        }
    }

    void update(sf::RenderWindow& window) {}




    void render(sf::RenderWindow& window) {
        if (!visible) return;

        sf::Vector2f center = window.getView().getCenter();

        // 선택창 크기: 화면 절반 높이
        sf::Vector2f boxSize(800.f, 400.f);
        background.setSize(boxSize);

        // 선택창 위치: 화면 상단 절반 기준 중앙
        sf::Vector2f bgPos(center.x - boxSize.x / 2.f, center.y - boxSize.y + 200.f);
        background.setPosition(bgPos);

        // 제목 위치: 선택창 상단 여백 + 가운데 정렬
        title.setPosition({ center.x - title.getLocalBounds().size.x / 2.f, bgPos.y + 20.f });

        // 포켓몬 스프라이트 배치 (2줄 5개) - background 기준 정렬
        for (int i = 0; i < sprites.size(); ++i) {
            sf::Vector2f offset((i % 5) * 130.f, (i / 5) * 130.f);
            /*sf::Vector2f spritePos = bgPos + sf::Vector2f(80.f, 70.f) + offset;
            sprites[i].setPosition(spritePos);
            borders[i].setPosition(spritePos);*/
            // 테두리 정중앙 좌표로 sprite 위치 조정
            sf::Vector2f pos = bgPos + sf::Vector2f(80.f, 70.f) + offset;

            // sprite 중앙 정렬 (보정값은 sprite 텍스처 크기 기준)
            sf::FloatRect bounds = sprites[i].getLocalBounds();
            sprites[i].setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
            sprites[i].setPosition({ pos.x + 64.f, pos.y + 64.f }); // border 중앙

            // 테두리는 그대로
            borders[i].setPosition(pos);

        }

        // 그리기
        window.draw(background);
        window.draw(title);
        for (int i = 0; i < sprites.size(); ++i) {
            window.draw(sprites[i]);
            window.draw(borders[i]);
        }
    }
};