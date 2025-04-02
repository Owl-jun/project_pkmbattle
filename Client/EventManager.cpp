#include "pch.h"
#include "EventManager.hpp"


EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::dispatch(const std::string& tag, const std::string& data) {
    std::lock_guard<std::mutex> lock(eventMutex);
    eventMap[tag].push_back(data);
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
