#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H


#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <map>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    bool Init(); // Khởi tạo hệ thống âm thanh SDL_mixer
    bool LoadMusic(const std::string& path);
    bool LoadSoundEffect(const std::string& path, const std::string& effect_name);// Tải một file hiệu ứng âm thanh từ đường dẫn path và gán tên effect_name
    void PlayMusic(int loop = -1); // Phát nhạc nền với số lần lặp (loop)


    void PlaySoundEffect(const std::string& effect_name);// Phát hiệu ứng âm thanh được gán với effect_name
    void PauseMusic();// Tạm dừng nhạc nền đang phát
    void ResumeMusic();// Tiếp tục phát nhạc nền đã bị tạm dừng
    void StopMusic();// Dừng hoàn toàn nhạc nền (khác với Pause, không thể Resume)
    void Free();
    bool IsMusicLoaded() const { return music_ != nullptr; }// Kiểm tra xem nhạc nền có được tải thành công không
    bool IsPaused() const { return Mix_PausedMusic() == 1; }// Kiểm tra xem nhạc nền có đang bị tạm dừng không
    const std::string& GetCurrentMusicPath() const { return current_music_path_; }// Lấy đường dẫn của file nhạc nền hiện tại

private:
    Mix_Music* music_;
    std::map<std::string, Mix_Chunk*> sound_effects_;
    // Một map để lưu trữ các hiệu ứng âm thanh, với key là tên và value là con trỏ Mix_Chunk*

    std::string current_music_path_;

};

#endif
