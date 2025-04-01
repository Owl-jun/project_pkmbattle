#include "EventManager.hpp"
#include <sstream>

EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::dispatch(const std::string& msg) {
    std::istringstream iss(msg);
    std::string tag;
    iss >> tag; // 첫 단어를 태그(명령어)로 사용

    std::lock_guard<std::mutex> lock(eventMutex);
    eventMap[tag].push_back(msg);
}

std::vector<std::string> EventManager::getEvents(const std::string& tag) {
    std::lock_guard<std::mutex> lock(eventMutex);
    if (eventMap.find(tag) == eventMap.end()) return {};
    return eventMap[tag]; // 복사본 반환
}

void EventManager::clearEvents(const std::string& tag) {
    std::lock_guard<std::mutex> lock(eventMutex);
    eventMap.erase(tag);
}
