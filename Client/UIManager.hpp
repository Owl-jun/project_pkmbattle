#pragma once
#include "BaseUI.hpp"
#include <vector>
#include "UIButton.hpp"

class UIManager {
private:
    std::vector<BaseUI*> uiElements;

public:
    void addElement(BaseUI* element) {
        uiElements.push_back(element);
    }

    // -----------------------------------------------------------------------------
    // Loop 3종
    void handleInput(const sf::Event& event, sf::RenderWindow& window) {
        if (event.is<sf::Event::KeyPressed>() && KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Tab)) {
            bool shiftHeld = KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::LShift);
            focusStep(shiftHeld ? -1 : 1);
            return;
        }
        if (event.is<sf::Event::KeyPressed>()) {
            if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Up)) {
                focusStep(-1);
                return;
            }
            else if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Down)) {
                focusStep(1);
                return;
            }
        }
        for (auto& elem : uiElements)
            elem->handleInput(event, window);
    }

    void update(sf::RenderWindow& window) {
        for (auto& elem : uiElements)
            elem->update(window);
    }

    void render(sf::RenderWindow& window) {
        for (auto& elem : uiElements)
            elem->render(window);
    }
    // -----------------------------------------------------------------------------

    void focusStep(int dir = 1) {
        int count = uiElements.size();
        int current = -1;

        for (int i = 0; i < count; ++i) {
            if (uiElements[i]->isFocusable() && uiElements[i]->isFocused()) {
                current = i;
                uiElements[i]->setFocus(false);
                break;
            }
        }

        for (int offset = 1; offset <= count; ++offset) {
            int next = (current + offset * dir + count) % count;
            if (uiElements[next]->isFocusable()) {
                uiElements[next]->setFocus(true);
                break;
            }
        }
    }

    // 버튼용
    void triggerFocusedElement() {
        for (auto& elem : uiElements) {
            if (elem->isFocusable() && elem->isFocused()) {
                if (auto* button = dynamic_cast<UIButton*>(elem)) {
                    button->click();
                    break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------

    const std::vector<BaseUI*>& getElements() const {
        return uiElements;
    }

    int getElementIndex(BaseUI* elem) const {
        for (int i = 0; i < uiElements.size(); ++i) {
            if (uiElements[i] == elem) return i;
        }
        return -1;
    }

    void removeAllElements() {
        uiElements.clear(); // delete는 하지 않고 리스트만 비움
    }

    // ---------------------------------------------------------------------
    // 메모리관리
    void clear() {
        for (auto& elem : uiElements)
            delete elem;
        uiElements.clear();
    }

    ~UIManager() {
        clear();
    }
    // ---------------------------------------------------------------------

};
