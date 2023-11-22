#pragma once
#include <random>

#include "ConsoleGame.h"
#include "GameType.h"
#include "GameUtils.h"

class Item {
   private:
    GameType::ItemType _type;
    ConsoleGame::AniSprite _sprite;
    float _x, _y;

   public:
    Item() = default;
    Item(
        GameType::ItemType type,
        const ConsoleGame::AniSprite& sprite,
        float x,
        float y
    );
    Item(GameType::ItemType type, const ConsoleGame::AniSprite& sprite);

    void Init(
        float x,
        float y,
        GameType::ItemType type,
        const ConsoleGame::AniSprite& sprite
    );

    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
    void UpdateSprite(float deltaTime);

    void SetY(float y);

    ConsoleGame::Box GetHitBox() const;
    float GetBottomY() const;
    float GetY() const;
    float GetX() const;
    GameType::ItemType GetType() const;
};