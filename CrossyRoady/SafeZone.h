#pragma once
#pragma once
#include "Lane.h"

class SafeZone : public Lane {
    ConsoleGame::Sprite _blockSprite;

   public:
    SafeZone() = default;

    SafeZone(
        float y,
        const ConsoleGame::Sprite& safeSprite,
        const ConsoleGame::Sprite& blockSprite
    )
        : Lane(y, blockSprite.GetHitBox(), safeSprite, GameType::LaneType::SAFE)
    {
        _blockSprite = blockSprite;
        Init();
    }

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override
    {
        size_t listSize = entityList.size();
        for (size_t i = 0; i < listSize; ++i) {
            _blockSprite.Paint(canvas, {(int)entityList[i], entityDrawY});
            GameUtils::DrawHitbox(canvas, GetHitBox(i));
        }
    }

    ConsoleGame::Box GetHitBox(size_t ind) const
    {
        ConsoleGame::Box hitbox = _blockSprite.GetHitBox();
        hitbox.coord.x += entityList[ind];
        hitbox.coord.y = entityY - hitbox.coord.y;

        return hitbox;
    }
};