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

    SafeZone(
        float y,
        ConsoleGame::Vec2 dim,
        bool isLeftToRight,
        bool hasItem,
        const std::vector<float>& enList
    );

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;
    void SetSprite(const ConsoleGame::Sprite& sprite);

    ConsoleGame::Box GetHitBox(size_t ind) const;
};