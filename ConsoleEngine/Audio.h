#pragma once

#include <filesystem>
#include <string>
#pragma comment(lib, "Winmm.lib")

namespace ConsoleGame {
    class Audio {
        bool _isPlaying = false;
        const size_t thiss = (size_t)this;
       public:
        Audio() = default;
        Audio(std::filesystem::path file);

        Audio(Audio&&) = delete;
        Audio(const Audio&) = delete;

        void Open(std::filesystem::path file);
        bool IsPlaying();
        void Play(bool fromStart = true, bool repeat = false);
        void Pause();
        void Resume();
        void Stop();
        void Close();
        void ChangeSong(std::filesystem::path file);
        ~Audio();
    };

    void PlayAndForget(std::filesystem::path path);
}  // namespace ConsoleGame
