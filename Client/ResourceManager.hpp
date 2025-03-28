// ResourceManager.hpp
#pragma once
#include <unordered_map>


class ResourceManager {
private:
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;

    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    static ResourceManager& getInstance();
    
    void init();
    sf::Texture& getTexture(const std::string& path);
    sf::Font& getFont(const std::string& path);

    void clear(); 
};
