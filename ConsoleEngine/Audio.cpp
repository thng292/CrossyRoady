#include "Audio.h"

#include <Windows.h>

#include <cstdint>
#include <format>

namespace ConsoleGame {

    std::mutex Audio::queueLock{};
    std::condition_variable Audio::bell{};
    std::queue<Audio::Cmd> Audio::commandBuffer{};
    std::jthread Audio::worker = std::jthread([&](std::stop_token stoken) {
        std::mutex cvm;
        static char sendCommand[40] = {0};
        char* commandStr            = nullptr;
        while (1) {
            std::unique_lock lock(cvm);
            bell.wait(lock, [&] { return not stoken.stop_requested(); });
            if (stoken.stop_requested()) {
                return;
            }
            Cmd command;
            while (1) {
                {
                    std::lock_guard lck(queueLock);
                    if (commandBuffer.empty()) {
                        break;
                    }
                    command = commandBuffer.front();
                    commandBuffer.pop();
                }
                switch (command.action) {
                    case Action::Play:
                        if (command.fromStart) {
                            if (command.repeat) {
                                commandStr = (char*)"play %llu from 0 repeat";
                            } else {
                                commandStr = (char*)"play %llu from 0";
                            }
                        } else {
                            if (command.repeat) {
                                commandStr = (char*)"play %llu repeat";
                            } else {
                                commandStr = (char*)"play %llu";
                            }
                        }
                        snprintf(
                            sendCommand,
                            sizeof(sendCommand),
                            commandStr,
                            command.alias
                        );
                        mciSendStringA(sendCommand, 0, 0, 0);
                        break;
                    case Action::Pause:
                        mciSendStringA(
                            std::format("pause {}", command.alias).c_str(),
                            0,
                            0,
                            0
                        );
                        break;
                    case Action::Resume:
                        mciSendStringA(
                            std::format("resume {}", command.alias).c_str(),
                            0,
                            0,
                            0
                        );
                        break;
                    case Action::Close:
                        mciSendStringA(
                            std::format("close {}", command.alias).c_str(),
                            0,
                            0,
                            0
                        );
                        break;
                    case Action::Stop:

                        mciSendStringA(
                            std::format("stop {}", command.alias).c_str(),
                            0,
                            0,
                            0
                        );
                        break;
                }
            }
        }
    });

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
        std::lock_guard lock(queueLock);
        commandBuffer.push(Cmd{.action = Action::Pause, .alias = thiss});
        bell.notify_all();
        _isPlaying = false;
    }

    Audio::Audio(std::filesystem::path file) { Open(file); }

    bool Audio::IsPlaying() { return _isPlaying; }

    void Audio::Play(bool fromStart, bool repeat)
    {
        std::lock_guard lock(queueLock);
        commandBuffer.push(Cmd{
            .action    = Action::Pause,
            .repeat    = repeat,
            .fromStart = fromStart,
            .alias     = thiss});
        bell.notify_all();
        _isPlaying = true;
    }

    void Audio::Pause()
    {
        std::lock_guard lock(queueLock);
        commandBuffer.push(Cmd{.action = Action::Pause, .alias = thiss});
        bell.notify_all();
        _isPlaying = false;
    }

    void Audio::Resume()
    {
        std::lock_guard lock(queueLock);
        commandBuffer.push(Cmd{.action = Action::Pause, .alias = thiss});
        bell.notify_all();
        _isPlaying = true;
    }

    void Audio::Stop()
    {
        std::lock_guard lock(queueLock);
        commandBuffer.push(Cmd{.action = Action::Pause, .alias = thiss});
        bell.notify_all();
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
