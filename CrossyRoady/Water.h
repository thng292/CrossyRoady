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
        bool isLeftToRight,
        std::vector<float> enList = std::vector<float>()
    );

    Water(
        float y,
        ConsoleGame::Vec2 dim,
        bool isLeftToRight,
        bool hasItem,
        const std::vector<float>& enList
    );

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;
    void SetSprite(const ConsoleGame::Sprite& logSprite);

    ConsoleGame::Box GetHitBox(size_t ind) const override;
};