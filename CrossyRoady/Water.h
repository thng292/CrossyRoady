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
    );

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;

    ConsoleGame::Box GetHitBox(size_t ind) const override;
};