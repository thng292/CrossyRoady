#pragma once
#include <random>
#include <vector>

#include "ConsoleGame.h"
#include "GameType.h"
#include "Lane.h"

class Road : public Lane {
   private:
    GameType::MobType _type;
    ConsoleGame::Sprite _roadSprite;
    ConsoleGame::AniSprite _mobSprite;

   public:
    Road() = default;

    Road(
        int y,
        int mobWidth,
        int mobHeight,
        GameType::MobType type,
        const ConsoleGame::Sprite& roadSprite,
        const ConsoleGame::AniSprite& mobSprite
    )
        : Lane(y, mobWidth, mobHeight, GameType::LaneType::PATH)
    {
        _type = type;
        _roadSprite = roadSprite;
        _mobSprite = mobSprite;
    }

    void UpdateSprite(float deltaTime)
    {
        _mobSprite.AutoUpdateFrame(deltaTime);
    }

    void Draw(ConsoleGame::AbstractCanvas* canvas) const override
    {
        // for (int i = 0; i < 384; i += 48) {
        _roadSprite.Paint(canvas, {yPos, 0});
        //}
        for (auto x : entityList) {
            _mobSprite.Paint(canvas, {yPos - 16, x});
        }
    }
};