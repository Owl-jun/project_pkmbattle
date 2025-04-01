#include "pch.h"
#include "GameManager.h"
#include "KeyManager.h"
#include "SceneManager.hpp"
#include "TitleScene.hpp"
#include "worldScene.hpp"
#include "LoginScene.hpp"
#include "OpeningScene.hpp"
#include "TimeManager.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"


GameManager::GameManager()
    : window(sf::VideoMode({ 800, 600 }), "PKM BATTLE") // 윈도우 타이틀 및 해상도설정
    , muteControl({ 720,500 })
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
    // 네트워크연결 , 테스트 완료 후 로그인시 연결하는걸로 변경
    NetworkManager::getInstance();
    NetworkManager::getInstance().connect("127.0.0.1", "9000");   // 연결문제 해결되면 주석해제
    std::string response;
    while (response.empty()) {
        response = NetworkManager::getInstance().receive();
    }
    int myId = -1;
    if (!response.empty()) {
        std::istringstream iss(response);
        std::string type;
        iss >> type;

        if (type == "ID") {
            iss >> myId;
            NetworkManager::getInstance().setMyId(myId);
            std::cout << "[Client] 내 ID는 " << NetworkManager::getInstance().getMyId() << "\n";
        }
        else {
            std::cout << "[Client] Unknown server response: " << response << "\n";
        }
    }
    // -------------------
    SoundManager::getInstance().playMusic("C:/Source/project_pkmbattle/Client/assets/track1.mp3");  // 사운드매니저
    ResourceManager::getInstance().init();
    // 주의할 점. 절대 같은 씬 또 만들면 안됨
    SceneManager::getInstance().registerScene("opening", new OpeningScene());
    SceneManager::getInstance().registerScene("title", new TitleScene());
    SceneManager::getInstance().registerScene("login", new LoginScene());
    // --------------------------------------
    SceneManager::getInstance().changeScene("opening");    // 초기화면 설정
}

void GameManager::update() { 
    TimeManager::getInstance().tick();
    KeyManager::getInstance().update();
    muteControl.update(window);
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
        {
            //--------------------------------------
            int count = 0;
            bool saved = false;

            while (count < 3) {
                NetworkManager::getInstance().send("EXIT\n");

                std::string exitResponse = NetworkManager::getInstance().receive_block();

                if (exitResponse == "EXIT_OK") {
                    std::cout << "[Client] 서버가 정상적으로 저장함\n";
                    //window.close();
                    saved = true;
                    break;
                }
                else {
                    std::cerr << "[Client] 서버에서 데이터 저장 실패! (" << count + 1 << "/3)\n";
                    count++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
            }

            if (!saved) {
                std::cerr << "[Client] 서버 응답 없음. 강제 종료합니다.\n";
            }
            
            window.close();
            //--------------------------------------

        }



        KeyManager::getInstance().handleEvent(*event);
        muteControl.handleEvent(*event,window);
        SceneManager::getInstance().handleInput(*event, window);    
    }
    SceneManager::getInstance().update(window);                    
}

void GameManager::render() {  
    window.clear();
    SceneManager::getInstance().render(window);
    if (!(SceneManager::getInstance().getCurScene() == SceneManager::getInstance().getScene("world")))
    {
        muteControl.render(window);
    }
    window.display();
}

void GameManager::run() {
    init();
    while (window.isOpen()) {
        update();   // 정보를 최신화
        render();   // 화면에 출력하는 기능.
    }
}
