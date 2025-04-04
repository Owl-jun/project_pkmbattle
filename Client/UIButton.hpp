#pragma once
#include "BaseUI.hpp"
#include "KeyManager.hpp"
#include <functional>

class UIButton : public BaseUI {
private:
    sf::RectangleShape shape;
    sf::Font font;
    sf::Text text;
    std::function<void()> onClick;

    sf::Color defaultColor;
    bool focused = false;
    bool enableHoverEffect = true;

public:
    UIButton()
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , text(font, "", 24)
    {}

    UIButton(const sf::Vector2f& pos,
        const sf::Vector2f& size,
        const std::string& label,
        unsigned int textSize,
        sf::Color Color,
        std::function<void()> clickFunc)
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , onClick(clickFunc)
        , text(font,label,textSize)
        , defaultColor(Color)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Color);
        text.setFillColor(sf::Color::Black);

        const auto bounds = text.getLocalBounds();
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
              shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    // wstring 생성자 (한글 깨짐 방지용)
    UIButton(const sf::Vector2f& pos,
        const sf::Vector2f& size,
        const std::wstring& label,
        unsigned int textSize,
        sf::Color Color,
        std::function<void()> clickFunc)
        : font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"))
        , onClick(clickFunc)
        , text(font, label, textSize)
        , defaultColor(Color)
    {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Color);
        text.setFillColor(sf::Color::Black);

        const auto bounds = text.getLocalBounds();  // 텍스트 크기를 가져옴
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });   // 텍스트 중심을 기준으로 위치를 설정
        text.setPosition(
         {shape.getPosition().x + shape.getSize().x / 2.f,
         shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    // ---------------------------------------------------------------------------
    // Loop 3종
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouse &&
                mouse->button == sf::Mouse::Button::Left &&
                shape.getGlobalBounds().contains(mousePos)) {
                if (onClick) { onClick(); }
            }
        }

        if (event.is<sf::Event::KeyPressed>()) {
            if (KeyManager::getInstance().isKeyPressed(sf::Keyboard::Key::Enter)) {
                if (onClick && isFocused()) { onClick(); }
            }
        }
    }
    
    void update(sf::RenderWindow& window) override {
        if (!enableHoverEffect) return;

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(defaultColor);
        }
    }

    void render(sf::RenderWindow& window) override {
        window.draw(shape);
        window.draw(text);
    }

    // ---------------------------------------------------------------------------

    // 게터 및 세터, Click

    void setHoverEffect(bool enable) {
        enableHoverEffect = enable;
    }

    sf::Vector2f getSize() const {
        return shape.getSize();
    }

    sf::RectangleShape& getShape() {
        return shape;
    }

    void setFunc(std::function<void()> onClickFunc)
    {
        onClick = onClickFunc;
    }

    void setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
        text.setPosition(
            { shape.getPosition().x + shape.getSize().x / 2.f,
            shape.getPosition().y + shape.getSize().y / 2.f }
        );
    }

    void setFillColor(sf::Color color) {
        shape.setFillColor(color);
    }

    void setString(const std::string& string) {
        text.setString(string);
    }

    void setString(const std::wstring& wstring) {
        text.setString(wstring);
    }

    void click() {
        if (onClick) onClick();
    }

    // ------------------------------------------------------------------------
    // 포커스 관련 
    bool isFocusable() const override { return true; }
    bool isFocused() const override { return focused; }

    void setFocus(bool focus) override {
        focused = focus;
        if (focus)
            shape.setOutlineColor(sf::Color::Yellow);
        else
            shape.setOutlineColor(sf::Color::Transparent);
        shape.setOutlineThickness(focus ? 3.f : 0.f);
    }
    // ------------------------------------------------------------------------

};
