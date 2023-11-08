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
        float y,
        float mobWidth,
        float mobHeight,
        GameType::MobType type,
        ConsoleGame::Sprite& roadSprite,
        const ConsoleGame::AniSprite& mobSprite
    )
        : Lane(y, mobWidth, mobHeight, GameType::LaneType::PATH)
    {
        _type = type;
        _roadSprite = roadSprite;
        _mobSprite = mobSprite;
        entityDrawY = laneDrawY + entityHeight / 2 + 5;
        entityFeetY = entityDrawY + entityHeight;
    }

    void UpdateSprite(float deltaTime)
    {
        _mobSprite.AutoUpdateFrame(deltaTime);
    }

    void DrawLane(ConsoleGame::AbstractCanvas* canvas) const override
    {
        for (int i = 0; i < ConsoleGame::_CONSOLE_WIDTH_; i += 32) {
            _roadSprite.Paint(canvas, {i, laneDrawY});
        }
    }

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override
    {
        for (auto x : entityList) {
            _mobSprite.Paint(canvas, {(int)x, entityDrawY});
        }
    }

    float GetHeight() const override
    {
        return std::max(
            _roadSprite.GetDim().height, _mobSprite.GetDim().height
        );
    }

    void SetY(float y) override
    {
        laneY = y;
        laneDrawY = ConsoleGame::_CONSOLE_HEIGHT_ * 2 - laneY;
        entityDrawY = laneDrawY + entityHeight / 2 + 5;
        entityFeetY = entityDrawY + entityHeight;
    }
};