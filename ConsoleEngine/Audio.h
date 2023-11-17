#pragma once

#include <atomic>
#include <filesystem>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#pragma comment(lib, "Winmm.lib")

namespace ConsoleGame {
    class Audio {
        enum class Action { Play, Pause, Resume, Stop, Close, Noop };

        struct Cmd {
            Action action;
            bool repeat;
            bool fromStart;
            size_t alias;
        };

        static std::condition_variable bell;
        static std::mutex queueLock;
        static std::queue<Cmd> commandBuffer;
        static std::jthread worker;
        bool _isPlaying    = false;
        const size_t thiss = (size_t)this;

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

    void PlayAndForget(std::filesystem::path path);
}  // namespace ConsoleGame
