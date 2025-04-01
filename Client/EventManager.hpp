#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

class EventManager {
private:
    EventManager() = default;

    // 메시지 분류 저장소: <"TAG", {"msg1","msg2",...}>
    std::unordered_map<std::string, std::vector<std::string>> eventMap;
    std::mutex eventMutex;

public:
    static EventManager& getInstance();

    void dispatch(const std::string& msg); // 수신된 메시지를 분류
    std::vector<std::string> getEvents(const std::string& tag); // 태그별 메시지 조회
    void clearEvents(const std::string& tag); // 사용 완료된 메시지 삭제

};
