#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() : music_(nullptr) {
}

SoundManager::~SoundManager() {
    Free();
}

bool SoundManager::Init() {
    // Khởi tạo SDL_mixer với tần số 44100 Hz
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}
bool SoundManager::LoadMusic(const std::string& path) {
    if (music_ != nullptr) {
        Mix_FreeMusic(music_);
        music_ = nullptr;
    }
    music_ = Mix_LoadMUS(path.c_str());
    if (music_ == nullptr) {
        std::cout << "Failed to load music! Path: " << path << " Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    current_music_path_ = path;
    return true;
}

bool SoundManager::LoadSoundEffect(const std::string& path, const std::string& effect_name) {
    // Tải hiệu ứng âm thanh
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr) {
        std::cout << "Failed to load sound effect! Path: " << path << " Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Lưu vào map với tên hiệu ứng
    sound_effects_[effect_name] = sound;
    return true;
}

void SoundManager::PlayMusic(int loop) {
    if (music_ != nullptr) {
        Mix_PlayMusic(music_, loop);
    }
}

void SoundManager::PlaySoundEffect(const std::string& effect_name) {
    auto it = sound_effects_.find(effect_name);
    if (it != sound_effects_.end()) {
        Mix_PlayChannel(-1, it->second, 0); // Phát trên kênh đầu tiên có sẵn
    } else {
        std::cout << "Sound effect not found: " << effect_name << std::endl;
    }
}

void SoundManager::PauseMusic() {
    Mix_PauseMusic();
}

void SoundManager::ResumeMusic() {
    Mix_ResumeMusic();
}

void SoundManager::StopMusic() {
    Mix_HaltMusic();
}

void SoundManager::Free() {
    // Giải phóng âm thanh nền
    if (music_ != nullptr) {
        Mix_FreeMusic(music_);
        music_ = nullptr;
    }

    // Giải phóng các hiệu ứng âm thanh
    for (auto& pair : sound_effects_) {
        Mix_FreeChunk(pair.second);
    }
    sound_effects_.clear();

    // Đóng SDL_mixer
    Mix_CloseAudio();
}
