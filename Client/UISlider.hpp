/* // ���� ����// UISlider - �����̴� �ٸ� ����� Ŭ������ ����� ���ſ���

#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include <functional>
#include <iostream>
#include <SFML/Window/Export.hpp>
#include <SFML/System/Vector2.hpp>

class UISlider : public BaseUI {
private:
    sf::RectangleShape track;  // �����̴� ��
    sf::RectangleShape knob;   // ���� �ڵ�
    float minValue, maxValue;
    float* targetValue;  // ���� ���� ���� ������ ����

public:
    UISlider(const sf::Vector2f& pos, const sf::Vector2f& size, float* volume)
        : minValue(0.f), maxValue(100.f), targetValue(volume)
    {
        // �����̴� �� ����
        track.setPosition(pos);
        track.setSize(size);
        track.setFillColor(sf::Color(200, 200, 200, 255));

        // ���� �ڵ� ����
        knob.setSize({ 20.f, size.y + 10.f });
        knob.setFillColor(sf::Color(100, 100, 100, 255));

        updateKnob();  // �ڵ� ��ġ ������Ʈ
    }

    void updateKnob() {
        if (!targetValue) return;
        float percentage = (*targetValue - minValue) / (maxValue - minValue);
        float newX = track.getPosition().x + (percentage * track.getSize().x) - (knob.getSize().x / 2.f);
        float newY = track.getPosition().y - 5.f;
        knob.setPosition(sf::Vector2f(newX, newY));
    }

    // setPosition �޼��� ����
    void setPosition(const sf::Vector2f& position) {
        track.setPosition(position);
        updateKnob();
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        sf::View overlayView = window.getView();
        window.setView(overlayView);

        if (event.is<sf::Event::MouseButtonPressed>()) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos, overlayView);

            if (track.getGlobalBounds().contains(mouseWorldPos)) {
                float newVolume = minValue + ((mouseWorldPos.x - track.getPosition().x) / track.getSize().x) * (maxValue - minValue);
                *targetValue = std::clamp(newVolume, minValue, maxValue);
                updateKnob();
                std::cout << "���� ������: " << *targetValue << std::endl;
            }
        }
    }

    void update(sf::RenderWindow& window) override {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

            if (track.getGlobalBounds().contains(mouseWorldPos)) {
                float newVolume = minValue + ((mouseWorldPos.x - track.getPosition().x) / track.getSize().x) * (maxValue - minValue);
                *targetValue = std::clamp(newVolume, minValue, maxValue);
                updateKnob();
            }
        }
    }

    void render(sf::RenderWindow& window) override {
        window.draw(track);
        window.draw(knob);
    }
};*/


#pragma once
#include "pch.h"
#include "SoundManager.hpp"
#include "BaseUI.hpp"
#include <functional>
#include <SFML/Window/Export.hpp>

class UISlider : public BaseUI {
private:
    sf::RectangleShape track;  // �����̴� ��
    sf::RectangleShape knob;   // ���� �ڵ�
    float minValue, maxValue;
    float* targetValue;  // ���� ���� ���� ������ ����

public:
    UISlider(const sf::Vector2f& pos, const sf::Vector2f& size, float* volume)
        : minValue(0.f), maxValue(100.f), targetValue(volume)
    {
        // ���� �����̴� �� ����
        track.setPosition(pos);
        track.setSize(sf::Vector2f(size.y, size.x));
        track.setFillColor(sf::Color(200, 200, 200, 255));

        // ���� ���� �ڵ� ����
        knob.setSize({ size.y + 10.f, 20.f });  // �ʺ�� �����̴� �� �ʺ��, ���̴� ����
        knob.setFillColor(sf::Color(100, 100, 100, 255));

        updateKnob();  // �ڵ� ��ġ ������Ʈ
    }

    void updateKnob() {
        if (!targetValue) return;
        float percentage = (*targetValue - minValue) / (maxValue - minValue);
        float newY = track.getPosition().y + (track.getSize().y - (percentage * track.getSize().y)) - (knob.getSize().y / 2.f);
        float newX = track.getPosition().x - 5.f;  // �ڵ� �߾� ��ġ ����
        knob.setPosition(sf::Vector2f(newX, newY));
    }

    // setPosition �޼��� ����
    void setPosition(const sf::Vector2f& position) {
        track.setPosition(position);
        updateKnob();
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override {
        sf::View overlayView = window.getView();
        window.setView(overlayView);

        if (event.is<sf::Event::MouseButtonPressed>()) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos, overlayView);

            if (track.getGlobalBounds().contains(mouseWorldPos)) {
                // y ��ǥ�� ������ ��� (���콺 ���� �ø��� �� ����)
                float newVolume = minValue + ((track.getPosition().y + track.getSize().y - mouseWorldPos.y) / track.getSize().y) * (maxValue - minValue);
                *targetValue = std::clamp(newVolume, minValue, maxValue);
                updateKnob();
                std::cout << "���� ������: " << *targetValue << std::endl;

                SoundManager::getInstance().setVolume(true);
            }
        }
    }

    void update(sf::RenderWindow& window) override {
        if (SoundManager::getInstance().isPlaying())
        {
            track.setFillColor(sf::Color(200, 200, 200, 255));
            knob.setFillColor(sf::Color(100, 100, 100, 255));

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

                if (track.getGlobalBounds().contains(mouseWorldPos)) {
                    float newVolume = minValue + ((track.getPosition().y + track.getSize().y - mouseWorldPos.y) / track.getSize().y) * (maxValue - minValue);
                    *targetValue = std::clamp(newVolume, minValue, maxValue);
                    updateKnob();
                }
            }
        }
        else {
            track.setFillColor(sf::Color(200, 200, 200, 125));
            knob.setFillColor(sf::Color(100, 100, 100, 125));
        }

    }

    void render(sf::RenderWindow& window) override {
        window.draw(track);
        window.draw(knob);
    }
};
