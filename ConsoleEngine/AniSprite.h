#pragma once
#include <chrono>
#include <filesystem>
#include <vector>

#include "Abstract/AbstractCanvas.h"
#include "Common.h"

namespace ConsoleGame {
    class AniSprite {
       protected:
        std::vector<Color> data;
        Vec2 dim;
        std::filesystem::path filePath;
        float frameDuration;

        uint32_t playingFrame = 0;
        uint32_t totalFrame = 0;
        float timePassed = 0;
        bool playing = false;
        bool repeat = false;

       public:
        AniSprite() = default;
        AniSprite(std::filesystem::path path);

        Vec2 GetDim() const;
        const std::vector<Color>& GetData() const;

        void Play(bool repeat = false);
        bool IsPlaying() const;
        void Stop();
        void Load(std::filesystem::path path);
        void Paint(AbstractCanvas* canvas, Vec2 coord) const;
        void ResetFrame();
        void AdvanceFrame();
        void AutoUpdateFrame(float deltaTime);
    };
}  // namespace ConsoleGame
