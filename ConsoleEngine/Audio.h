#pragma once

#include <filesystem>
#include <string>

#include "Common.h"

namespace ConsoleGame {
    class Audio {
        Sound sound{};

       public:
        Audio() = default;
        Audio(std::filesystem::path file);

        Audio(Audio&&)      = delete;
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

}  // namespace ConsoleGame
