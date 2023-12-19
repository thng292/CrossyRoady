#include "Audio.h"

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>

#include "Logger.h"
#include "raylib.h"

namespace ConsoleGame {

    void Audio::Open(std::filesystem::path audioFile)
    {
        sound = LoadSound(audioFile.c_str());
    }

    void Audio::Close() { UnloadSound(sound); }

    Audio::Audio(std::filesystem::path file) { Open(file); }

    bool Audio::IsPlaying() { return IsSoundPlaying(sound); }

    void Audio::Play(bool fromStart, bool repeat) { PlaySound(sound); }

    void Audio::Pause() { PauseSound(sound); }

    void Audio::Resume() { ResumeSound(sound); }

    void Audio::Stop() { StopSound(sound); }

    void Audio::ChangeSong(std::filesystem::path file)
    {
        Close();
        Open(file);
    }

    Audio::~Audio() { Close(); }

}  // namespace ConsoleGame
