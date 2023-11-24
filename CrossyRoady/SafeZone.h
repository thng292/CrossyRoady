#pragma once
#pragma once
#include "Lane.h"

class SafeZone : public Lane {
    ConsoleGame::Sprite* _blockSprite;

   public:
    SafeZone() = default;

    void SafeInit();

    SafeZone(
        float y,
        ConsoleGame::Sprite* safeSprite,
        ConsoleGame::Sprite* blockSprite,
        bool isLeftToRight,
        bool isInitialSafe = false,
        std::vector<float> enList = std::vector<float>()
    );

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;

    ConsoleGame::Box GetHitBox(size_t ind) const;
};