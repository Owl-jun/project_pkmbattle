#pragma once
#include "pch.h"

class SoundManager {
private:
    sf::Music music;    // 멤버

    // 싱글톤패턴
    SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

public:
    // 외부에서 사용할 메서드
    static SoundManager& getInstance() {
        static SoundManager instance;
        return instance;
    }

    // 경로를 받아서 노래 틀어주는 함수
    bool playMusic(const std::string& path, float volume = 50.f, bool loop = true) {
        if (!music.openFromFile(path)) {
            std::cerr << "[SoundManager] Failed to load music: " << path << std::endl;
            return false;
        }
        // 볼륨크기
        music.setVolume(volume);
        // 반복재생 여부
        music.setLooping(loop);
        // 재생
        music.play();
        return true;
    }

    // 노래끄기기능
    void stopMusic() {
        music.stop();
    }

    // 노래 멈춤기능
    void pauseMusic() {
        music.pause();
    }

    // 노래 재개기능
    void resumeMusic() {
        if (music.getStatus() == sf::Music::Status::Paused)
            music.play();
    }

    // 볼륨조절기능
    void setVolume(float volume) {
        music.setVolume(volume);
    }

    // 진행중이니 (true false 반환)
    bool isPlaying() const {
        return music.getStatus() == sf::Music::Status::Playing;
    }
};
