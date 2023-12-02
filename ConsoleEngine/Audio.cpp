#include "Audio.h"

#include <Windows.h>

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>

#include "Logger.h"

namespace ConsoleGame {
    constexpr size_t QLEN         = 20;
    char commandBuffer[QLEN][128] = {0};
#ifdef _DEBUG
    char errBuffer[128] = {0};
#endif
    int head = 0;
    int tail = 0;
    std::atomic_bool hasJob;
    std::mutex queueLock;
    std::jthread audioThread([](std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            hasJob.wait(false);
            while (head != tail) {
                std::lock_guard lk(queueLock);
                auto err = mciSendStringA(commandBuffer[head], 0, 0, 0);
#ifdef _DEBUG
                LogDebug("{}", commandBuffer[head]);
                if (err != 0) {
                    mciGetErrorStringA(err, errBuffer, sizeof(errBuffer));
                    LogDebug("{}", errBuffer);
                }
#endif
                head = (head + 1) % QLEN;
            }
            hasJob.store(false);
        }
    });

    void Audio::Open(std::filesystem::path audioFile)
    {
        const char* device = 0;
        if (audioFile.extension().string() == ".wav") {
            device = "waveaudio";
        } else if (audioFile.extension().string() == ".mp3") {
            device = "mpegvideo";
        }
        auto file = audioFile.string();
        std::lock_guard lk(queueLock);
        snprintf(
            commandBuffer[tail],
            sizeof(commandBuffer[tail]),
            "open %s type %s alias %llu",
            file.c_str(),
            device,
            thiss
        );
        tail = (tail + 1) % QLEN;
        hasJob.store(true);
        hasJob.notify_all();
    }

    void Audio::Close()
    {
        _isPlaying = false;
        std::lock_guard lk(queueLock);
        snprintf(
            commandBuffer[tail],
            sizeof(commandBuffer[tail]),
            "close %llu",
            thiss
        );
        tail = (tail + 1) % QLEN;
        hasJob.store(true);
        hasJob.notify_all();
    }

    Audio::Audio(std::filesystem::path file) { Open(file); }

    bool Audio::IsPlaying() { return _isPlaying; }

    void Audio::Play(bool fromStart, bool repeat)
    {
        char* command = nullptr;
        if (fromStart) {
            if (repeat) {
                command = (char*)"play %llu from 0 repeat";
            } else {
                command = (char*)"play %llu from 0";
            }
        } else {
            if (repeat) {
                command = (char*)"play %llu repeat";
            } else {
                command = (char*)"play %llu";
            }
        }
        _isPlaying = true;
        std::lock_guard lk(queueLock);
        snprintf(
            commandBuffer[tail], sizeof(commandBuffer[tail]), command, thiss
        );
        tail = (tail + 1) % QLEN;
        hasJob.store(true);
        hasJob.notify_all();
    }

    void Audio::Pause()
    {
        _isPlaying = false;
        std::lock_guard lk(queueLock);
        snprintf(
            commandBuffer[tail],
            sizeof(commandBuffer[tail]),
            "pause %llu",
            thiss
        );
        tail = (tail + 1) % QLEN;
        hasJob.store(true);
        hasJob.notify_all();
    }

    void Audio::Resume()
    {
        _isPlaying = true;
        std::lock_guard lk(queueLock);
        snprintf(
            commandBuffer[tail],
            sizeof(commandBuffer[tail]),
            "resume %llu",
            thiss
        );
        tail = (tail + 1) % QLEN;
        hasJob.store(true);
        hasJob.notify_all();
    }

    void Audio::Stop()
    {
        std::lock_guard lk(queueLock);
        snprintf(
            commandBuffer[tail], sizeof(commandBuffer[tail]), "stop %llu", thiss
        );
        tail = (tail + 1) % QLEN;
        hasJob.store(true);
        hasJob.notify_all();
    }

    void Audio::ChangeSong(std::filesystem::path file)
    {
        Close();
        Open(file);
    }

    Audio::~Audio() { Close(); }

    void PlayAndForget(std::filesystem::path path)
    {
        PlaySound(path.c_str(), NULL, SND_ASYNC | SND_FILENAME);
    }
}  // namespace ConsoleGame
