#pragma once
#include "pch.h"
#include "SoundManager.hpp"
#include "BaseUI.hpp"
#include <functional>
#include <SFML/Window/Export.hpp>

class UISlider : public BaseUI {
private:
    sf::RectangleShape track;  // 슬라이더 바
    sf::RectangleShape knob;   // 조절 핸들
    float minValue, maxValue;
    float* targetValue;  // 실제 볼륨 값을 조작할 변수

public:
    UISlider(const sf::Vector2f& pos, const sf::Vector2f& size, float* volume)
        : minValue(0.f), maxValue(100.f), targetValue(volume)
    {
        // 세로 슬라이더 바 설정
        track.setPosition(pos);
        track.setSize(sf::Vector2f(size.y, size.x));
        track.setFillColor(sf::Color(200, 200, 200, 255));

        // 세로 조절 핸들 설정
        knob.setSize({ size.y + 10.f, 20.f });  // 너비는 슬라이더 바 너비로, 높이는 고정
        knob.setFillColor(sf::Color(100, 100, 100, 255));

        updateKnob();  // 핸들 위치 업데이트
    }

    void updateKnob() {
        if (!targetValue) return;
        float percentage = (*targetValue - minValue) / (maxValue - minValue);
        float newY = track.getPosition().y + (track.getSize().y - (percentage * track.getSize().y)) - (knob.getSize().y / 2.f);
        float newX = track.getPosition().x - 5.f;  // 핸들 중앙 위치 조정
        knob.setPosition(sf::Vector2f(newX, newY));
    }

    // setPosition 메서드 수정
    void setPosition(const sf::Vector2f& position) {
        track.setPosition(position);
        updateKnob();
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        sf::View overlayView = window.getView();
        window.setView(overlayView);

        if (event.is<sf::Event::MouseButtonPressed>()) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos, overlayView);

            if (track.getGlobalBounds().contains(mouseWorldPos)) {
                // y 좌표로 볼륨을 계산 (마우스 위로 올리면 값 증가)
                float newVolume = minValue + ((track.getPosition().y + track.getSize().y - mouseWorldPos.y) / track.getSize().y) * (maxValue - minValue);
                *targetValue = std::clamp(newVolume, minValue, maxValue);
                updateKnob();
                std::cout << "볼륨 조절됨: " << *targetValue << std::endl;

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
