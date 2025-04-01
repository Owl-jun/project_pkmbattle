// 배틀 로직
#include "pch.h"
#include "KeyManager.h"
#include "BaseScene.hpp"
#include "TitleScene.hpp"
#include "SceneManager.hpp"
#include "AnimatedObject.hpp"
#include "AnimationManager.hpp"
#include "ResourceManager.hpp"

class BattleManager : public BaseScene {
private:
    sf::Font font;
    sf::Text text;
    AnimationManager aniManager;
public:
    BattleManager()
        : font(ResourceManager::getInstance().getFont("C:/Source/project_pkmbattle/Client/fonts/POKEMONGSKMONO.TTF"))
    {
    };
};