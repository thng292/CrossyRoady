#pragma once
#pragma once
#include "Lane.h"

class SafeZone : public Lane {
    ConsoleGame::Sprite _blockSprite;

   public:
    SafeZone() = default;

    void SafeInit()
    {
        int paddingCnt = 3;
        int padX = 2;
        for (size_t i = 0; i < paddingCnt; ++i) {
            entityList.push_back(i * (entityWidth + padX));
        }
        for (size_t i = 0; i < paddingCnt + 1; ++i) {
            entityList.push_back(
                ConsoleGame::_CONSOLE_WIDTH_ - i * (entityWidth + padX)
            );
        }
    }

    SafeZone(
        float y,
        const ConsoleGame::Sprite& safeSprite,
        const ConsoleGame::Sprite& blockSprite,
        bool isLeftToRight,
        bool isInitialSafe = false
    )
        : Lane(
              y,
              blockSprite.GetDim(),
              safeSprite,
              GameType::LaneType::SAFE,
              isLeftToRight
          )
    {
        _blockSprite = blockSprite;
        if (isInitialSafe) {
            SafeInit();
        } else {
            Init();
        }
    }

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override
    {
        size_t listSize = entityList.size();
        for (size_t i = 0; i < listSize; ++i) {
            _blockSprite.Paint(canvas, {(int)entityList[i], entityDrawY});
            // GameUtils::DrawHitbox(canvas, GetHitBox(i));
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