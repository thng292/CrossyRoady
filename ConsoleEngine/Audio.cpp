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
        sound      = LoadSound(audioFile.string().c_str());
        isUnloaded = false;
    }

    void Audio::Close()
    {
        if (isUnloaded == false) {
            UnloadSound(sound);
            isUnloaded = true;
        }
    }

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
}

namespace ConsoleGame {

    std::vector<MusicStream*> MusicStream::instances{};

    MusicStream::MusicStream() {
        MusicStream::instances.push_back(this);
    }

    void MusicStream::Open(std::filesystem::path audioFile)
    {
        sound      = LoadMusicStream(audioFile.string().c_str());
        isUnloaded = false;
    }

    void MusicStream::Close()
    {
        if (isUnloaded == false) {
            UnloadMusicStream(sound);
            isUnloaded = true;
        }
    }

    MusicStream::MusicStream(std::filesystem::path file) { 
        Open(file); 
        MusicStream::instances.push_back(this); 
    }

    bool MusicStream::IsPlaying() { return IsMusicStreamPlaying(sound); }

    void MusicStream::Play(bool fromStart, bool repeat) { PlayMusicStream(sound); }

    void MusicStream::Pause() { PauseMusicStream(sound); }

    void MusicStream::Resume() { ResumeMusicStream(sound); }

    void MusicStream::Stop() { StopMusicStream(sound); }

    void MusicStream::ChangeSong(std::filesystem::path file)
    {
        Close();
        Open(file);
    }

    void MusicStream::UpdateStreamBuffer() { UpdateMusicStream(sound); }

    MusicStream::~MusicStream() {
        for (int i = 0; i < MusicStream::instances.size(); i++) {
            if (MusicStream::instances[i] == this) {
                MusicStream::instances.erase(MusicStream::instances.begin() + i);
                break;
            }
        }
        Close();
    }

}  // namespace ConsoleGame
