#include "Audio.h"

#include <Windows.h>

#include <cstdint>
#include <thread>

#include "Logger.h"

namespace ConsoleGame {
    char commandBuffer[512]     = {0};
    std::atomic<char*> _command = nullptr;
    std::atomic_bool isClose    = false;
    std::jthread audioThread([](std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            /*if (_command == nullptr) {
                continue;
            }*/
            _command.wait(nullptr);
            auto err = mciSendStringA(_command.load(), 0, 0, 0);
#ifdef _DEBUG
            if (err) {
                mciGetErrorStringA(err, commandBuffer, sizeof(commandBuffer));
                if (!isClose) {
                    LogDebug("{}", commandBuffer);
                }
                isClose = false;
            }
#endif
            _command.store(nullptr);
            _command.notify_all();
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
        _command.wait(commandBuffer);
        snprintf(
            commandBuffer,
            sizeof(commandBuffer),
            "open %s type %s alias %llu",
            file.c_str(),
            device,
            thiss
        );
        _command.store(commandBuffer);
        _command.notify_all();
        LogDebug("{}", commandBuffer);
    }

    void Audio::Close()
    {
        _command.wait(commandBuffer);
        isClose = true;
        snprintf(commandBuffer, sizeof(commandBuffer), "close %llu", thiss);
        _command.store(commandBuffer);
        _command.notify_all();
        _isPlaying = false;
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
        _command.wait(commandBuffer);
        snprintf(commandBuffer, sizeof(commandBuffer), command, thiss);
        _command.store(commandBuffer);
        _command.notify_all();
        LogDebug("{}", commandBuffer);
        _isPlaying = true;
    }

    void Audio::Pause()
    {
        _command.wait(commandBuffer);
        snprintf(commandBuffer, sizeof(commandBuffer), "pause %llu", thiss);
        _command.store(commandBuffer);
        _command.notify_all();
        LogDebug("{}", commandBuffer);
        _isPlaying = false;
    }

    void Audio::Resume()
    {
        _command.wait(commandBuffer);
        snprintf(commandBuffer, sizeof(commandBuffer), "resume %llu", thiss);
        _command.store(commandBuffer);
        _command.notify_all();
        LogDebug("{}", commandBuffer);
        _isPlaying = true;
    }

    void Audio::Stop()
    {
        _command.wait(commandBuffer);
        snprintf(commandBuffer, sizeof(commandBuffer), "stop %llu", thiss);
        _command.store(commandBuffer);
        _command.notify_all();
        LogDebug("{}", commandBuffer);
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
