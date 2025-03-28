#pragma once
#include "pch.h"

class SoundManager {
private:
    sf::Music music;    // ���

    // �̱�������
    SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

public:
    // �ܺο��� ����� �޼���
    static SoundManager& getInstance() {
        static SoundManager instance;
        return instance;
    }

    // ��θ� �޾Ƽ� �뷡 Ʋ���ִ� �Լ�
    bool playMusic(const std::string& path, float volume = 50.f, bool loop = true) {
        if (!music.openFromFile(path)) {
            std::cerr << "[SoundManager] Failed to load music: " << path << std::endl;
            return false;
        }
        // ����ũ��
        music.setVolume(volume);
        // �ݺ���� ����
        music.setLooping(loop);
        // ���
        music.play();
        return true;
    }

    // �뷡������
    void stopMusic() {
        music.stop();
    }

    // �뷡 ������
    void pauseMusic() {
        music.pause();
    }

    // �뷡 �簳���
    void resumeMusic() {
        if (music.getStatus() == sf::Music::Status::Paused)
            music.play();
    }

    // �����������
    void setVolume(float volume) {
        music.setVolume(volume);
    }

    // �������̴� (true false ��ȯ)
    bool isPlaying() const {
        return music.getStatus() == sf::Music::Status::Playing;
    }
};
