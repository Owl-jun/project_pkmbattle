#include "pch.h"
#include "ResourceManager.hpp"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::init() {
    std::vector<std::string> texturesToLoad = {
        "C:/Source/project_pkmbattle/Client/assets/introbg.png",
        "C:/Source/project_pkmbattle/Client/assets/introbird.png",
        "C:/Source/project_pkmbattle/Client/assets/kstext.png",
        "C:/Source/project_pkmbattle/Client/assets/hjtext.png",
        "C:/Source/project_pkmbattle/Client/assets/MADEBY.png",
        "C:/Source/project_pkmbattle/Client/assets/KHSJ.png",
        "C:/Source/project_pkmbattle/Client/assets/chansoo.png",
        "C:/Source/project_pkmbattle/Client/assets/bojeong.png",
        "C:/Source/project_pkmbattle/Client/assets/dongwan.png",
        "C:/Source/project_pkmbattle/Client/assets/seokjun.png",
        "C:/Source/project_pkmbattle/Client/assets/hyeongwoo.png",
        "C:/Source/project_pkmbattle/Client/assets/bigjun.png",
        "C:/Source/project_pkmbattle/Client/assets/gugu.png",
        "C:/Source/project_pkmbattle/Client/assets/worldMap.png",
        "C:/Source/project_pkmbattle/Client/assets/player00.png",
        "C:/Source/project_pkmbattle/Client/assets/player01.png",
        "C:/Source/project_pkmbattle/Client/assets/player02.png",
        "C:/Source/project_pkmbattle/Client/assets/player03.png",
        "C:/Source/project_pkmbattle/Client/assets/player04.png",
        "C:/Source/project_pkmbattle/Client/assets/player05.png",
        "C:/Source/project_pkmbattle/Client/assets/player06.png",
        "C:/Source/project_pkmbattle/Client/assets/player07.png",
        "C:/Source/project_pkmbattle/Client/assets/player08.png",
        "C:/Source/project_pkmbattle/Client/assets/player09.png"
    };

    for (const auto& path : texturesToLoad) {
        getTexture(path);
    }

    getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF");
}

sf::Texture& ResourceManager::getTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it == textures.end()) {
        // 직접 map에 생성 후 로딩 (복사 없이 안전)
        if (!textures[path].loadFromFile(path)) {
            std::cerr << "Failed to load texture: " << path << std::endl;
            throw std::runtime_error("Texture load failed: " + path);
        }
    }
    return textures[path];
}

sf::Font& ResourceManager::getFont(const std::string& path) {
    auto it = fonts.find(path);
    if (it == fonts.end()) {
        if (!fonts[path].openFromFile(path)) {
            std::cerr << "Failed to load font: " << path << std::endl;
        }
    }
    return fonts[path];
}

void ResourceManager::clear() {
    textures.clear();
    fonts.clear();
}
