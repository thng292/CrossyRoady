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
        : Lane(y, mobSprite.GetHitBox(), roadSprite, GameType::LaneType::ROAD)
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
        size_t listSize = entityList.size();
        for (size_t i = 0; i < listSize; ++i) {
            _mobSprite.Paint(canvas, {(int)entityList[i], entityDrawY});
            GameUtils::DrawHitbox(canvas, GetHitBox(i));
        }
    }

    GameType::MobType GetMobType() { return _type; }

    ConsoleGame::Box GetHitBox(size_t ind) const
    {
        ConsoleGame::Box hitbox = _mobSprite.GetHitBox();
        hitbox.coord.x += entityList[ind];
        hitbox.coord.y = entityY - hitbox.coord.y;
        return hitbox;
    }
};
