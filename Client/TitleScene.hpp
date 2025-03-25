#pragma once
#include "BaseScene.hpp"


class TitleScene : public BaseScene {
private:
    sf::Font font;
    sf::Text title;

public:
    TitleScene() : font("C:/Windows/Fonts/arial.ttf"), title(font, "PKM BATTLE", 60) {} // 씨빨 자슥들 폰트 왜 인식쳐 못함 하

    void init() override {        
        title.setFillColor(sf::Color::White);
        title.setPosition({ 200, 250 });
    }

    void update(sf::RenderWindow& window) override {
        // 키 입력 받아서 씬 전환 처리 가능
    }

    void render(sf::RenderWindow& window) override {
        window.draw(title);
    }
};
