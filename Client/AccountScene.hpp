#pragma once
#include "pch.h"
#include "baseScene.hpp"
#include "GameManager.hpp"
#include "SceneManager.hpp"
#include "UIManager.hpp"
#include "UITextBox.hpp"
#include "UIButton.hpp"


class AccountScene : public BaseScene {
private:
	UIManager uiManager;
	sf::Font font = sf::Font(ResourceManager::getInstance().getFontByName("POKEMONGSKMONO.TTF"));
	sf::Sprite bg = sf::Sprite(ResourceManager::getInstance().getTextureByName("introbg.png"));
	sf::Sprite back = sf::Sprite(ResourceManager::getInstance().getTextureByName("back.png"));
	sf::Text title = sf::Text(font, L"회원가입", 48);
	sf::RectangleShape textBg = sf::RectangleShape({ 170.f,80.f });
	bool isView = false;
	bool isOk;
	sf::Text oksign = sf::Text(font, L"가입완료!", 48);
	sf::Text falsesign = sf::Text(font, sf::String(L"아이디중복!"), 48);
	sf::RenderWindow& window = GameManager::getInstance().getWindow();

	float timer = 2.f;
	float changetimer = 2.f;
	bool timerOn = false;
	sf::Text nametag = sf::Text(font, L"NICKNAME", 24);
	sf::Text idtag = sf::Text(font, L"ID", 24);
	sf::Text pwtag = sf::Text(font, L"PW", 24);
	UITextBox* nameBox = nullptr;
	UITextBox* idBox = nullptr;
	UITextBox* pwBox = nullptr;
	std::string nickname;
	std::string id;
	std::string pw;
public:
	AccountScene() {}
	
	void init() override 
	{
		bg.setScale({ 1.0f,1.0f });
		back.setPosition({ 11.f,11.f });
		title.setPosition({(window.getSize().x /2.f) - (title.getGlobalBounds().size.x/2.f) , 25.f});
		//textBg.setPosition({ window.getSize().x / 2.f - 75.f, window.getSize().y / 2.f - 35.f});
		textBg.setPosition({ 300.f, 300.f });
		textBg.setFillColor(sf::Color::Color(0,0,0,125));

		oksign.setPosition({ textBg.getPosition().x + 10.f , textBg.getPosition().y - 40.f});
		falsesign.setPosition({ textBg.getPosition().x + 10.f , textBg.getPosition().y - 40.f});
		
		nameBox = new UITextBox({ (window.getSize().x / 2.f) - 100.f , 200.f }, { 200.f, 30.f }, 24);
		nametag.setPosition({ (window.getSize().x / 2.f) - 230.f , 185.f });
		nametag.setFillColor(sf::Color::White);
		nametag.setOutlineThickness(1.1f);

		idBox = new UITextBox({ (window.getSize().x / 2.f) - 100.f , 250.f }, { 200.f, 30.f }, 24);
		idtag.setPosition({ (window.getSize().x / 2.f) - 230.f , 235.f });
		idtag.setFillColor(sf::Color::White);
		idtag.setOutlineThickness(1.1f);

		pwBox = new UITextBox({ (window.getSize().x / 2.f) - 100.f , 300.f }, { 200.f, 30.f }, 24);
		pwtag.setPosition({ (window.getSize().x / 2.f) - 230.f , 285.f });
		pwtag.setFillColor(sf::Color::White);
		pwtag.setOutlineThickness(1.1f);

		uiManager.addElement(nameBox);
		uiManager.addElement(idBox);
		uiManager.addElement(pwBox);
		uiManager.addElement(new UIButton({ (window.getSize().x / 2.f) - 100.f , 350.f }, { 150.f, 50.f },L"가입하기", 48, sf::Color::Color(0,0,0,120),
			[&]() {
				nickname = nameBox->getInput();
				id = idBox->getInput();
				pw = pwBox->getInput();

				auto clean = [](const std::string& str) {
					std::string result;
					for (char c : str) {
						if (!isspace(static_cast<unsigned char>(c)) && c != 27)
							result += c;
					}
					return result;
					};
				nickname = clean(nickname);
				id = clean(id);
				pw = clean(pw);
				std::string toSend = "CREATEACCOUNT " + nickname + " " + id + " " + pw + "\n";
				NetworkManager::getInstance().send(toSend);
			}));
		uiManager.addElement(new UIButton({ 10.f,10.f }, { 50.f,50.f }, L"", 36, sf::Color::Yellow, []() {
			SceneManager::getInstance().changeScene("login");
			}));
	}
	void handleEvent(std::string tag, std::string msg) override {
		std::istringstream iss(msg);
		std::string response;
		iss >> response;
		std::cout << "[AccountScene] 수신 tag : " << tag << " response : " << msg << std::endl;
		if (tag == "CREATEACCOUNT") {
			if (response == "OK") { 
				nameBox->clear();
				idBox->clear();
				pwBox->clear();
				isOk = true; 
				timerOn = true; 
			}
			else if (response == "FALSE") { isOk = false; }
			isView = true;
			timer = 2.f;
			EventManager::getInstance().clearEvents(tag);
		}
		else {
			std::cout << "잘못된 tag 형식입니다." << std::endl;
		}
	}

	void handleInput(const sf::Event& event, sf::RenderWindow& window) override {
		uiManager.handleInput(event,window);
	}

	void update(sf::RenderWindow& window) override {
		for (const std::string& tag : { "CREATEACCOUNT" }) {
			auto events = EventManager::getInstance().getEvents(tag);
			for (const auto& msg : events) {
				handleEvent(tag, msg);
			}
		}
		float dt = TimeManager::getInstance().getDeltaTime();
		timer -= dt;
		if (timerOn) { changetimer -= dt; }
		if (timerOn && changetimer <= 0.f) { 
			timerOn = false;
			changetimer = 1.5f;
			SceneManager::getInstance().changeScene("login"); 
		}
		uiManager.update(window);
	}
	void render(sf::RenderWindow& window) override {
		window.draw(bg);
		window.draw(title);
		window.draw(nametag);
		window.draw(idtag);
		window.draw(pwtag);
		uiManager.render(window);
		window.draw(back);
		if (isView) {
			window.draw(textBg);
			if (isOk) { window.draw(oksign); }
			else { window.draw(falsesign); }
			if (timer <= 0.f) { isView = false; }
		}
	}
};