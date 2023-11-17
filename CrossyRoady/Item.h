#pragma once
#include <random>

#include "ConsoleGame.h"
#include "GameType.h"
#include "GameUtils.h"

class Item {
   private:
    GameType::ItemType _type;
    ConsoleGame::Sprite _sprite;
    float _x, _y;

   public:
    Item() = default;
    Item(
        GameType::ItemType type,
        const ConsoleGame::Sprite& sprite,
        float x,
        float y
    );
    Item(GameType::ItemType type, const ConsoleGame::Sprite& sprite);

    void Init(
        float y, GameType::ItemType type, const ConsoleGame::Sprite& sprite
    );
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;

    void SetY(float y);

    ConsoleGame::Box GetHitBox() const;
    float GetBottomY() const;
    float GetY() const;
    GameType::ItemType GetType() const;
};