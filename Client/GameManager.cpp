#include "pch.h"
#include "GameManager.hpp"
#include "KeyManager.hpp"
#include "SceneManager.hpp"
#include "TitleScene.hpp"
#include "worldScene.hpp"
#include "LoginScene.hpp"
#include "OpeningScene.hpp"
#include "TimeManager.hpp"
#include "SoundManager.hpp"


GameManager::GameManager()
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE") // 윈도우 타이틀 및 해상도설정
{
}

GameManager& GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

sf::RenderWindow& GameManager::getWindow()
{
    return window;
}

void GameManager::init() {
    // 서버 연결 및 메시지 수신스레드 RUN // 
    // TEST시 : localhost , 서버주소 210.119.12.82 //
    NetworkManager::getInstance();
    NetworkManager::getInstance().connect("localhost", "9000");
    NetworkManager::getInstance().startReceiveLoop();
    // ------------------------------------------------------------------------
    // 리소스 매니저 (Assets 폴더 모든 파일 등록)
    // 이미지파일 : .png | .jpg  /// 폰트파일 : .ttf
    ResourceManager::getInstance().initAuto();  
    // ------------------------------------------------------------------------
    // 모든 씬 생성 : 키워드 [ opening , title , login , world , battle ]
    SceneManager::getInstance().registerAll();
    SceneManager::getInstance().changeScene("opening");    // 초기화면 설정
    // ------------------------------------------------------------------------
}

// 서버 패킷 핸들러
void GameManager::handleEvent(std::string tag, std::string msg)
{
    if (tag == "EXIT") {
        int count = 3;
        if (msg == "EXIT_OK") {
            window.close();
        }
        else {
            NetworkManager::getInstance().send("EXIT\n");
            --count;
            if (count <= 0) { 
                std::cout << "저장실패, 강제 종료합니다." << std::endl;
                window.close(); 
            }
        }
    }
    else {
        std::cout << "잘못된 서버응답 입니다." << tag << std::endl;
    }
}

// -----------------------------------------------------------------
// 게임LOOP
// INPUT 처리
void GameManager::handleInput()
{
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        KeyManager::getInstance().handleInput(*event);
        SceneManager::getInstance().handleInput(*event, window);
        if (event->is<sf::Event::Closed>()) {
            NetworkManager::getInstance().send("EXIT\n");
        }
    }
}

void GameManager::update() { 
    // 메시지큐 -> 이벤트매니저 전달
    while (NetworkManager::getInstance().hasMessage()) {
        EventManager::getInstance().dispatch(NetworkManager::getInstance().popMessage());
    }

    // 수신 패킷이름 지정
    for (const std::string& tag : { "EXIT" }) {
        auto events = EventManager::getInstance().getEvents(tag);
        for (const auto& msg : events) {
            handleEvent(tag, msg);
        }
        EventManager::getInstance().clearEvents(tag);
    }

    // 매니저 UPDATE
    TimeManager::getInstance().tick();
    KeyManager::getInstance().update();
    SceneManager::getInstance().update(window);                    
}

void GameManager::render() {  
    window.clear();
    SceneManager::getInstance().render(window);
    window.display();
}
// -----------------------------------------------------------------

void GameManager::run() {
    init();
    while (window.isOpen()) {
        handleInput(); 
        update();   
        render();   
    }
}
