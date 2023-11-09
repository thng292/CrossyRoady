// #pragma once
#include "ConsoleGame.h"
#include "Lane.h"

class Water : public Lane {
    // Inherited via Lane
    ConsoleGame::Sprite _logSprite;

   public:
    Water() = default;

    Water(
        float y,
        float mobWidth,
        float mobHeight,
        const ConsoleGame::Sprite& waterSprite,
        const ConsoleGame::Sprite& logSprite
    )
        : Lane(y, mobWidth, mobHeight, waterSprite, GameType::LaneType::WATER)
    {
        _logSprite = logSprite;
        Init();
    }

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override
    {
        for (auto x : entityList) {
            _logSprite.Paint(canvas, {(int)x, entityDrawY});
        }
    }
};