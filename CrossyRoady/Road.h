#pragma once
#include <random>
#include <vector>

#include "ConsoleGame.h"
#include "GameType.h"
#include "Lane.h"

class Road : public Lane {
   private:
    GameType::MobType _type;
    ConsoleGame::AniSprite _mobSprite;

   public:
    Road() = default;

    Road(
        float y,
        float mobWidth,
        float mobHeight,
        GameType::MobType type,
        ConsoleGame::Sprite& roadSprite,
        const ConsoleGame::AniSprite& mobSprite
    )
        : Lane(y, mobWidth, mobHeight, roadSprite, GameType::LaneType::ROAD)
    {
        _type = type;
        _mobSprite = mobSprite;
        _mobSprite.Play(1);
        Init();
    }

    void UpdateSprite(float deltaTime)
    {
        _mobSprite.AutoUpdateFrame(deltaTime);
    }

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override
    {
        for (auto x : entityList) {
            _mobSprite.Paint(canvas, {(int)x, entityDrawY});
            GameUtils::DrawHitbox(
                canvas, x, entityDrawY, _mobSprite.GetHitBox()
            );
        }
    }
};
