#include "Audio.h"

#include <Windows.h>

#include <cstdint>
#include <format>

namespace ConsoleGame {

    void Audio::Open(std::filesystem::path audioFile)
    {
        std::string command;
        if (audioFile.extension().string() == ".wav") {
            command = std::format(
                "open {} type {} alias {}",
                audioFile.string(),
                "waveaudio",
                thiss
            );
        } else if (audioFile.extension().string() == ".mp3") {
            command = std::format(
                "open {} type {} alias {}",
                audioFile.string(),
                "mpegvideo",
                thiss
            );
        }
        mciSendStringA(command.c_str(), 0, 0, 0);
    }

    void Audio::Close()
    {
        mciSendStringA(std::format("close {}", thiss).c_str(), 0, 0, 0);
        _isPlaying = false;
    }

    Audio::Audio(std::filesystem::path file) { Open(file); }

    bool Audio::IsPlaying() { return _isPlaying; }

    void Audio::Play(bool fromStart, bool repeat)
    {
        static char sendCommand[40] = {0};
        char* command               = nullptr;
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
        snprintf(sendCommand, sizeof(sendCommand), command, thiss);
        mciSendStringA(sendCommand, 0, 0, 0);
        _isPlaying = true;
    }

    void Audio::Pause()
    {
        mciSendStringA(std::format("pause {}", thiss).c_str(), 0, 0, 0);
        _isPlaying = false;
    }

    void Audio::Resume()
    {
        mciSendStringA(std::format("resume {}", thiss).c_str(), 0, 0, 0);
        _isPlaying = true;
    }

    void Audio::Stop()
    {
        mciSendStringA(std::format("stop {}", thiss).c_str(), 0, 0, 0);
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
