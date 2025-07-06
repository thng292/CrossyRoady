#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Common.h"

namespace ConsoleGame {
    class Audio {
        bool isUnloaded = false;
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

    class MusicStream {
        bool isUnloaded = false;
        Music sound{};

       public:
        static std::vector<MusicStream*> instances;
        MusicStream();
        MusicStream(std::filesystem::path file);

        MusicStream(Audio&&)      = delete;
        MusicStream(const Audio&) = delete;

        void Open(std::filesystem::path file);
        bool IsPlaying();
        void Play(bool fromStart = true, bool repeat = false);
        void Pause();
        void Resume();
        void Stop();
        void Close();
        void ChangeSong(std::filesystem::path file);
        void UpdateStreamBuffer();
        ~MusicStream();
    };

}  // namespace ConsoleGame
