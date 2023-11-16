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
        Box hitBox;
        Vec2 dim;
        float frameDuration   = 0;

        uint32_t playingFrame = 0;
        uint32_t totalFrame   = 0;
        float timePassed      = 0;
        bool playing          = false;
        bool repeat           = false;

        void findHitBox();

       public:
        AniSprite() = default;
        AniSprite(std::filesystem::path path);

        Vec2 GetDim() const;
        const Box& GetHitBox() const;
        void EditHitBox(Vec2 coordOffset, Vec2 dimOffset);
        float GetFrameDuration();
        void SetFrameDuration(float dur);
        const std::vector<Color>& GetData() const;

        void Load(std::filesystem::path path);
        void Unload();
        void Play(bool repeat = false);
        bool IsPlaying() const;
        void Stop();
        void Draw(AbstractCanvas* canvas, Vec2 coord) const;
        void ResetFrame();
        void AdvanceFrame();
        void AutoUpdateFrame(float deltaTime);
    };
}  // namespace ConsoleGame
