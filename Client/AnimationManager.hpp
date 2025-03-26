#pragma once
#include "pch.h"
#include "AnimatedObject.hpp"

class AnimationManager {
private:
    std::vector<AnimatedObject> objects;
    std::vector<std::function<void(AnimatedObject&, float)>> updateFuncs;

public:
    void add(const AnimatedObject&& obj, std::function<void(AnimatedObject&, float)> updateFunc) {
        objects.push_back(obj);
        updateFuncs.push_back(updateFunc);
    }

    bool updateAll(float dt) {
        bool anyFinished = false;
        for (size_t i = 0; i < objects.size(); ++i) {
            updateFuncs[i](objects[i], dt);
            if (objects[i].isFinished()) {
                anyFinished = true;
            }
        }
        return anyFinished;
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
