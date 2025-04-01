#include "pch.h"
#include "ResourceManager.hpp"
#include <filesystem>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

// 경로 탐색
void ResourceManager::initAuto() {
    namespace fs = std::filesystem;
    fs::path current = fs::current_path();

    // assets 폴더를 자동 탐색 (위로 올라가면서 찾기)
    while (current.has_parent_path()) {
        fs::path tryAssets = current / "Client" / "Assets";
        if (fs::exists(tryAssets)) {
            std::cout << "[ResourceManager] Assets loaded from: " << tryAssets << '\n';
            init(tryAssets.string());
            return;
        }
        current = current.parent_path();
    }
    throw std::runtime_error("Assets directory not found!");
}

void ResourceManager::init(const std::string& assetDir = "Client/Assets/") {
    namespace fs = std::filesystem;

    for (const auto& entry : fs::recursive_directory_iterator(assetDir)) {
        if (!entry.is_regular_file()) continue;

        std::string path = entry.path().string();
        std::string filename = entry.path().filename().string();

        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            sf::Texture tex;
            if (tex.loadFromFile(path)) {
                textures[filename] = std::move(tex);
            }
            else {
                std::cerr << "[ResourceManager] Failed to load texture: " << path << '\n';
            }
        }
        else if (entry.path().extension() == ".ttf") {
            sf::Font font;
            if (font.openFromFile(path)) {
                fonts[filename] = std::move(font);
            }
            else {
                std::cerr << "[ResourceManager] Failed to load font: " << path << '\n';
            }
        }
    }
}

sf::Texture& ResourceManager::getTextureByName(const std::string& filename) {
    if (textures.find(filename) == textures.end()) {
        throw std::runtime_error("Texture not found: " + filename);
    }
    return textures[filename];
}

sf::Font& ResourceManager::getFontByName(const std::string& filename) {
    if (fonts.find(filename) == fonts.end()) {
        throw std::runtime_error("Font not found: " + filename);
    }
    return fonts[filename];
}

void ResourceManager::clear() {
    textures.clear();
    fonts.clear();
}
