#pragma once
#include "BaseUI.hpp"
#include "UIButton.hpp"
#include "TimeManager.hpp"
#include <functional>
#include <vector>
#include <memory>

class FPSControl : public BaseUI {
private:
    sf::RectangleShape shape;
    sf::Text text;  // FPS ǥ�� �ؽ�Ʈ
    sf::Font& font; // ��Ʈ ����
    std::vector<std::unique_ptr<UIButton>> buttons; // FPS ���� ��ư��
    sf::Color defaultColor;
    std::string label;

public:
    // FPSControl ������
    FPSControl(const std::string& label
        , const sf::Vector2f& position
        , const sf::Vector2f& size
        , sf::Color color
        , sf::Font& sharedFont
        , std::function<void(int)> fpsChangeFunc)
        : font(sharedFont)
        , label(label)
        , defaultColor(color)
        , text(sharedFont, label, 24)
    {
        // ��� ����
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(color);

        // FPS ǥ�� �ؽ�Ʈ ����
        text.setFillColor(sf::Color::Black);
        text.setPosition({ shape.getPosition().x + shape.getSize().x / 2.f,
                           shape.getPosition().y + shape.getSize().y / 2.f });

        std::vector<int> fpsValues = { 10, 20, 30 };
        float offsetY = 2.f;

        for (int fps : fpsValues) {
            buttons.push_back(std::make_unique<UIButton>(
                sf::Vector2f(position.x, position.y),                // ��ư�� ��ġ
                sf::Vector2f(100.f, 40.f),                           // ��ư�� ũ��
                "FPS: " + std::to_string(fps),                       // ��ư�� ǥ���� �ؽ�Ʈ
                sf::Color(214, 181, 106, 255),                       // ��ư�� ����
                font,                                                // ��ư ��Ʈ
                [this, fps]() {                                      // ��ư Ŭ�� ����
                    TimeManager::getInstance().setFPS(fps);          // FPS ����
                    text.setString("FPS: " + std::to_string(fps));   // ��ư Ŭ�� �� �ؽ�Ʈ �ٲ��(10,20...)
                },
                24                                                   // ��ư �ؽ�Ʈ ũ�� ����
            ));
            offsetY += 50.f;                                         // ���� ��ư�� 50��ŭ �Ʒ�
        }
    }

    sf::Vector2f getSize() const {
        return shape.getSize();
    }

    void setPosition(const sf::Vector2f& position) {
        // FPSControl�� ��� ��ġ ����
        shape.setPosition(position);
        text.setPosition({ position.x + shape.getSize().x / 2.f - text.getLocalBounds().size.x / 2.f,
                           position.y - 70.f });//

        // ��ư���� ��ġ�� FPSControl�� ��ġ�� �°� ������
        float buttonOffsetY = 0.5f;  // ù ��° ��ư ����
        for (auto& button : buttons) {
            button->setPosition({ position.x + (shape.getSize().x - button->getSize().x) / 2.f,
                                  position.y + buttonOffsetY });
            buttonOffsetY += 50.f;  // �� ��ư ������ ������ 50���� ����
        }
    }

    // �̺�Ʈ ó�� (��ư Ŭ�� ����)
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        for (auto& button : buttons) {
            button->handleEvent(event, window);
        }
    }

    // ������Ʈ (�ʿ� �� ��ư ���� ����)
    void update(sf::RenderWindow& window) override {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(defaultColor);
        }
    }

    // ȭ�鿡 FPS �ؽ�Ʈ �� ��ư ������
    void render(sf::RenderWindow& window) override {
        window.draw(text);
        window.draw(shape);
        for (auto& button : buttons) {
            button->render(window);  // ��ư ������
        }
    }
};