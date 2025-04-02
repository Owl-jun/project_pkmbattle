#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class ResourceManager {
private:
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;

    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    static ResourceManager& getInstance();
    void initAuto();
    void init(const std::string& assetDir = "Client/Assets/");

    // filename은 확장자를 포함합니다.
    sf::Texture& getTextureByName(const std::string& filename);
    sf::Font& getFontByName(const std::string& filename);

    void clear(); 
};
