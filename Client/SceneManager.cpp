#include "pch.h"
#include "baseScene.hpp"
#include "SceneManager.hpp"
#include "OpeningScene.hpp"
#include "TitleScene.hpp"
#include "LoginScene.hpp"
#include "AccountScene.hpp"
#include "worldScene.hpp"

void SceneManager::handleEvent(std::string tag, std::string msg)
{
	if (currentScene)
		currentScene->handleEvent(tag, msg);
}

void SceneManager::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
	if (currentScene)
		currentScene->handleInput(event, window);
}

void SceneManager::update(sf::RenderWindow& window)
{
	if (currentScene)
		currentScene->update(window);
	applyPendingScene();
}

void SceneManager::render(sf::RenderWindow& window)
{
	if (currentScene)
		currentScene->render(window);
}

void SceneManager::registerAll() {
	std::cout << "[Register] OpeningScene\n";
	registerScene("opening", new OpeningScene);

	std::cout << "[Register] TitleScene\n";
	registerScene("title", new TitleScene);

	std::cout << "[Register] LoginScene\n";
	registerScene("login", new LoginScene);

	std::cout << "[Register] accountScene\n";
	registerScene("account", new AccountScene);

	std::cout << "[Register] WorldScene\n";
	registerScene("world", new worldScene);
}


void SceneManager::registerScene(const std::string& key, BaseScene* scene)
{
	scenes[key] = scene;
	scene->init();
}

void SceneManager::changeScene(const std::string& key)
{
	pendingSceneKey = key;
}

void SceneManager::applyPendingScene()
{
	if (!pendingSceneKey.empty()) {
		auto it = scenes.find(pendingSceneKey);
		if (it != scenes.end()) {
			// std::cout << "[SceneManager] Scene Change : " << it->first << "\n";
			currentScene = it->second;
		}
	}
}

BaseScene* SceneManager::getScene(const std::string& key)
{
	auto it = scenes.find(key);
	return (it != scenes.end()) ? it->second : nullptr;
}

BaseScene* SceneManager::getCurScene()
{
	return currentScene;
}

void SceneManager::cleanup()
{
	for (auto& [key, scene] : scenes) {
		delete scene;
	}
	scenes.clear();
}

SceneManager::~SceneManager()
{
	cleanup();
}

