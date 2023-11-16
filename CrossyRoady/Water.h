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
        const ConsoleGame::Sprite& waterSprite,
        const ConsoleGame::Sprite& logSprite,
        bool isLeftToRight
    )
        : Lane(
              y,
              logSprite.GetDim(),
              waterSprite,
              GameType::LaneType::WATER,
              isLeftToRight
          )
    {
        _logSprite = logSprite;
        Init();
    }

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override
    {
        size_t listSize = entityList.size();

        for (size_t i = 0; i < listSize; ++i) {
            _logSprite.Draw(canvas, {(int)entityList[i], entityDrawY});
            // GameUtils::DrawHitbox(canvas, GetHitBox(i));
        }
    }

    ConsoleGame::Box GetHitBox(size_t ind) const
    {
        ConsoleGame::Box hitbox = _logSprite.GetHitBox();
        hitbox.coord.x += entityList[ind];
        hitbox.coord.y = entityY - hitbox.coord.y;

        return hitbox;
    }
};