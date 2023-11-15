#pragma once
#pragma once
#include "Lane.h"

class SafeZone : public Lane {
    ConsoleGame::Sprite _blockSprite;

   public:
    SafeZone() = default;

    void SafeInit();

    SafeZone(
        float y,
        const ConsoleGame::Sprite& safeSprite,
        const ConsoleGame::Sprite& blockSprite,
        bool isLeftToRight,
        bool isInitialSafe = false
    );

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;

    ConsoleGame::Box GetHitBox(size_t ind) const;
};