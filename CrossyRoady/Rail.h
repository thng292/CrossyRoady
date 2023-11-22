#pragma once
#include <random>
#include <vector>

#include "ConsoleGame.h"
#include "GameType.h"
#include "Lane.h"

class Rail : public Lane {
   private:
    GameType::MobType _type;
    ConsoleGame::AniSprite _mobSprite;

   public:
    Rail() = default;

    Rail(
        float y,
        GameType::MobType type,
        ConsoleGame::Sprite& roadSprite,
        const ConsoleGame::AniSprite& mobSprite,
        bool isLeftToRight,
        std::vector<float> enList = std::vector<float>()
    );

    Rail(
        float y,
        ConsoleGame::Vec2 dim,
        GameType::MobType type,
        bool isLeftToRight,
        bool hasItem,
        const std::vector<float>& enList
    );

    void Init() override;
    void CreateEntity() override;
    void UpdatePos(float deltaTime) override;
    void UpdateSprite(float deltaTime);
    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;

    void SetSprite(const ConsoleGame::AniSprite& sprite);

    GameType::MobType GetMobType();
    ConsoleGame::Box GetHitBox(size_t ind) const;
};
