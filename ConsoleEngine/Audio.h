#pragma once

#include <Windows.h>

#include <filesystem>
#pragma comment(lib, "Winmm.lib")

namespace ConsoleGame {
    class Audio {
        std::filesystem::path audioFile;
        bool _isPlaying = false;

        void Open();
        void Close();
       public:
        Audio(std::filesystem::path file);

        Audio(Audio&&) = delete;
        Audio(const Audio&) = delete;

        bool IsPlaying();
        void Play(bool fromStart = true, bool repeat = false);
        void Pause();
        void Resume();
        void Stop();
        void ChangeSong(std::filesystem::path file);
        ~Audio();
    };
}  // namespace ConsoleGame
