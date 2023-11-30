#pragma once
#include <random>
#include <vector>

#include "GameType.h"
#include "Lane.h"

class Rail : public Lane {
   private:
    GameType::MobType _type;
    ConsoleGame::AniSprite* _mobSprite;
    ConsoleGame::Sprite* _arrow;
    ConsoleGame::Audio* _warningSfx;

    float flashTimer = 0;
    bool warning = false;

   public:
    Rail() = default;

    Rail(
        float y,
        GameType::MobType type,
        ConsoleGame::Sprite* arrow,
        ConsoleGame::AniSprite* mobSprite,
        bool isLeftToRight,
        std::vector<float> enList = std::vector<float>(),
        ConsoleGame::Sprite* roadSprite = nullptr,
        ConsoleGame::Audio* warningSfx = nullptr
    );

    void Init() override;
    void CreateEntity() override;
    void UpdatePos(float deltaTime) override;
    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override;
    void DrawWarning(ConsoleGame::AbstractCanvas* canvas) const;

    GameType::MobType GetMobType();
    ConsoleGame::Box GetHitBox(size_t ind) const;
};
