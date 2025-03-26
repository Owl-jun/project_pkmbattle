#pragma once
#include "pch.h"
#include "AnimatedObject.hpp"

class AnimationManager {
private:
    std::vector<AnimatedObject> objects;

public:
    void add(const AnimatedObject&& obj) {
        objects.push_back(obj);
    }

    void updateAll(float dt,float windowWidth = 0.f) {
        for (auto& obj : objects) {
            obj.update(dt, windowWidth);
        }
    }

    void renderAll(sf::RenderWindow& window) {
        for (auto& obj : objects) {
            obj.draw(window);
        }
    }

    std::vector<AnimatedObject>& getObjects() {
        return objects;
    }
};
