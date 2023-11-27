#pragma once
#include <random>
#include <vector>

#include "ConsoleGame.h"
#include "GameType.h"
#include "Lane.h"

class Road : public Lane {
   private:
    GameType::MobType _type;
    ConsoleGame::AniSprite* _mobSprite;

   public:
    Road() = default;
    Road(
        float y,
        GameType::MobType type,
        ConsoleGame::Sprite* roadSprite,
        ConsoleGame::AniSprite* mobSprite,
        bool isLeftToRight,
        std::vector<float> enList = std::vector<float>()
    );

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;

    GameType::MobType GetMobType();
    ConsoleGame::Box GetHitBox(size_t ind) const;
};
