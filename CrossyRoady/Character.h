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
    GameType::Direction _direc = GameType::UP;
    int maxHealth;
    int curHealth;
    float _speed;

    float x;
    float y;

    ConsoleGame::AniSprite* currentSprite;
    ConsoleGame::AniSprite leftSprite, upSprite, rightSprite, downSprite;
    CharStat* charStat = ((CharStat*)&R.CharsStat);

    void UpdateHitBox(ConsoleGame::AniSprite& sprite);

   public:
    Character() = default;

    void Init(GameType::CharaType type, float xIn, float yIn);
    void InitSave(
        GameType::CharaType type,
        float xIn,
        float yIn,
        int health,
        int mhealth,
        int speed,
        GameType::Direction direc
    );

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

    GameType::Direction GetDirec() const;
    int GetCurHealth() const;
    int GetMaxHealth() const;
    int getSpeed() const;
    float GetBottomY() const;
    ConsoleGame::Box GetHitBox() const;
    float GetX() const;
    float GetY() const;
    GameType::CharaType GetType() const;
    ConsoleGame::Vec2 GetDrawCoord() const;
};
