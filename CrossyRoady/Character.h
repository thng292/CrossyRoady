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

    void Init(GameType::CharaType type, float xIn, float yIn);

    void MoveLeft(float dist);
    void MoveRight(float dist);
    void MoveUp(float dist);
    void MoveDown(float dist);

    void Draw(ConsoleGame::AbstractCanvas*& canvas) const;
    void UnloadSprites();
    void LoadSprites(GameType::CharaType type);

    void SetCurHealth(int health);
    void SetMaxHealth(int health);
    void SetSpeed(const double& speed);
    void SetSpriteRight();
    void SetSpriteLeft();
    void SetSpriteUp();
    void SetSpriteDown();

    void UpdateFrame(float deltaTime);
    void AdvanceFrame();
    void ResetSprite();

    int GetCurHealth() const;
    int getMaxHealth() const;
    int getSpeed() const;
    float GetBottomY() const;
    ConsoleGame::Box GetHitBox() const;
    float GetX() const;
    float GetY() const;
    ConsoleGame::Vec2 GetDrawCoord() const;
};
