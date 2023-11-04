#include "Audio.h"

#include <Windows.h>

#include <cstdint>
#include <format>

namespace ConsoleGame {

    void Audio::Open(std::filesystem::path audioFile)
    {
        std::wstring command;
        if (audioFile.extension().string() == ".wav") {
            command = std::format(
                L"open {} type {} alias {}",
                audioFile.wstring(),
                L"waveaudio",
                (uintptr_t)this
            );
        } else if (audioFile.extension().string() == ".mp3") {
            command = std::format(
                L"open {} type {} alias {}",
                audioFile.wstring(),
                L"mpegvideo",
                (uintptr_t)this
            );
        }
        mciSendString(command.c_str(), 0, 0, 0);
    }

    void Audio::Close()
    {
        mciSendString(
            std::format(L"close {}", (uintptr_t)this).c_str(), 0, 0, 0
        );
        _isPlaying = false;
    }

    Audio::Audio(std::filesystem::path file) { Open(file); }

    bool Audio::IsPlaying() { return _isPlaying; }

    void Audio::Play(bool fromStart, bool repeat)
    {
        auto command = std::format(L"play {}", (uintptr_t)this);
        if (fromStart) {
            command += L" from 0";
        }
        if (repeat) {
            command += L" repeat";
        }
        mciSendString(command.c_str(), 0, 0, 0);
        _isPlaying = true;
    }

    void Audio::Pause()
    {
        mciSendString(
            std::format(L"pause {}", (uintptr_t)this).c_str(), 0, 0, 0
        );
        _isPlaying = false;
    }

    void Audio::Resume()
    {
        mciSendString(
            std::format(L"resume {}", (uintptr_t)this).c_str(), 0, 0, 0
        );
        _isPlaying = true;
    }

    void Audio::Stop()
    {
        mciSendString(
            std::format(L"stop {}", (uintptr_t)this).c_str(), 0, 0, 0
        );
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
