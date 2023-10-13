#include "Audio.h"

#include <Windows.h>

#include <cstdint>
#include <format>

void ConsoleGame::Audio::Open()
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

void ConsoleGame::Audio::Close() {
    mciSendString(std::format(L"close {}", (uintptr_t)this).c_str(), 0, 0, 0);
    _isPlaying = false;
}

ConsoleGame::Audio::Audio(std::filesystem::path file)
{
    audioFile = file;
    Open();
}

bool ConsoleGame::Audio::IsPlaying() { return _isPlaying; }

void ConsoleGame::Audio::Play(bool fromStart, bool repeat)
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

void ConsoleGame::Audio::Pause()
{
    mciSendString(std::format(L"pause {}", (uintptr_t)this).c_str(), 0, 0, 0);
    _isPlaying = false;
}

void ConsoleGame::Audio::Resume()
{
    mciSendString(std::format(L"resume {}", (uintptr_t)this).c_str(), 0, 0, 0);
    _isPlaying = true;
}

void ConsoleGame::Audio::Stop() {
    mciSendString(std::format(L"stop {}", (uintptr_t)this).c_str(), 0, 0, 0);
}

void ConsoleGame::Audio::ChangeSong(std::filesystem::path file) {
    audioFile = file;
    Close();
    Open();
}

ConsoleGame::Audio::~Audio() { Close(); }
