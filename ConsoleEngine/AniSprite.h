#pragma once
#include <chrono>
#include <filesystem>
#include <vector>

#include "Abstract/AbstractCanvas.h"
#include "Common.h"

namespace ConsoleGame {
    class AniSprite {
       public:
        static constexpr int durationMultiplier = 25;

       private:
        std::vector<Color> data;
        Vec2 dim;
        std::filesystem::path filePath;
        std::chrono::milliseconds frameDuration;

        uint8_t playingFrame = 0;
        uint8_t totalFrame = 0;
        float timePassed = 0;
        bool playing = false;
        bool repeat = false;

       public:
        AniSprite() = default;
        AniSprite(std::filesystem::path path);
        AniSprite(const AniSprite&);
        AniSprite(AniSprite&& other) noexcept;

        const AniSprite operator=(const AniSprite& other);

        Vec2 GetDim() const;
        const std::vector<Color>& GetData() const;

        void Play(bool repeat = false);
        void Stop();
        void Load(std::filesystem::path path);
        void Paint(AbstractCanvas* canvas, Vec2 coord) const;
        void ResetFrame();
        void AdvanceFrame();
        void AutoUpdateFrame(float deltaTime);
    };
}  // namespace ConsoleGame
