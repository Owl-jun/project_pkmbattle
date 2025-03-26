#pragma once
#include "BaseUI.hpp"
#include <vector>

class UIManager {
private:
    std::vector<BaseUI*> uiElements;

public:
    void addElement(BaseUI* element) {
        uiElements.push_back(element);
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        for (auto& elem : uiElements)
            elem->handleEvent(event, window);
    }

    void update(sf::RenderWindow& window) {
        for (auto& elem : uiElements)
            elem->update(window);
    }

    void render(sf::RenderWindow& window) {
        for (auto& elem : uiElements)
            elem->render(window);
    }

    void clear() {
        for (auto& elem : uiElements)
            delete elem;
        uiElements.clear();
    }

    ~UIManager() {
        clear();
    }
};
