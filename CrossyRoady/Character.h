#pragma once

#include <format>
#include <string_view>

#include "ConsoleGame.h"
#include "GameType.h"
#include "GameUtils.h"
#include "StringRes.h"

class Character {
   private:
    GameType::CharaType _type;
    int maxHealth;
    int curHealth;
    float _speed;

    float x;
    float y;
    int drawX;
    int drawY;

    ConsoleGame::AniSprite* currentSprite;
    ConsoleGame::AniSprite leftSprite, upSprite, rightSprite, downSprite;

    void UpdateHitBox(ConsoleGame::AniSprite& sprite);

   public:
    Character() = default;

    void Init(GameType::CharaType type);

    void MoveLeft(float deltaTime);

    void MoveRight(float deltaTime);

    void MoveUp(float deltaTime);

    void MoveDown(float deltaTime);

    void Draw(ConsoleGame::AbstractCanvas*& canvas) const;

    void SetCurHealth(int health);

    void SetMaxHealth(int health);

    void SetSpeed(const double& speed);

    int GetCurHealth() const;

    int getMaxHealth() const;

    int getSpeed() const;

    float GetBottomY() const;

    ConsoleGame::Box GetHitBox() const;

    float GetX() const;

    float GetY() const;

    ConsoleGame::Vec2 GetDrawCoord() const;
};
