#pragma once
#include "ConsoleGame.h"
#include "Lane.h"

class Water : public Lane {
    // Inherited via Lane
    ConsoleGame::Sprite* _logSprite;

   public:
    Water() = default;

    Water(
        float y,
        ConsoleGame::Sprite* waterSprite,
        ConsoleGame::Sprite* logSprite,
        bool isLeftToRight,
        std::vector<float> enList = std::vector<float>()
    );

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;

    ConsoleGame::Box GetHitBox(size_t ind) const override;
};