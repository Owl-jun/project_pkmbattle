#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "KeyManager.h"
#include <functional>

class SettingsOverlay {
private:
    sf::RectangleShape background;
    sf::Text title;
    sf::Font font;

    std::vector<std::unique_ptr<BaseUI>> uiElements;
    bool visible = false;

public:
    SettingsOverlay(const sf::Vector2f& size, const sf::Font& sharedFont)
        : font(sharedFont), title(font, "Settings", 36)
    {
        background.setSize(size);
        background.setFillColor(sf::Color(50, 50, 50, 180));
        background.setPosition({ 0.f, 0.f });

        title.setFillColor(sf::Color::Black);
        title.setPosition({ 30.f, 20.f });

        // ����: ���� ����, ������ ��ư �� �߰� ����
        // �̰��� UIButton �� UI �߰� ����
    }

    void toggle() {
        visible = !visible;
    }

    bool isVisible() const {
        return visible;
    }

    void setCenter(const sf::Vector2f& center) {
        background.setPosition({ center - background.getSize() / 2.f });
        title.setPosition({ center.x - title.getLocalBounds().size.x / 2.f, center.y - background.getSize().y / 2.f + 20.f });
        // ���� ��ư, �ؽ�Ʈ�ڽ� � center �������� ����
    }


    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (!visible) return;
        for (auto& ui : uiElements)
            ui->handleEvent(event, window);
    }

    void update(sf::RenderWindow& window) {
        if (!visible) return;
        for (auto& ui : uiElements)
            ui->update(window);
    }

    void render(sf::RenderWindow& window) {
        if (!visible) return;
        window.draw(background);
        window.draw(title);
        for (auto& ui : uiElements)
            ui->render(window);
    }

    void addElement(BaseUI* ui) {
        uiElements.emplace_back(ui);
    }

    void clear() {
        uiElements.clear();
    }
};