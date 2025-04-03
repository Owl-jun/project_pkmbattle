#pragma once
#include "pch.h"
#include "BaseUI.hpp"
#include "ResourceManager.hpp"

class UIChatIcon : public BaseUI {
private:
	sf::Font font = ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF");

public:
	UIChatIcon() {}
	
};