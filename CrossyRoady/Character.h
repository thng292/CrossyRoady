#pragma once

#include <format>
#include <string_view>

#include "ConsoleGame.h"
#include "GameType.h"
#include "StringRes.h"

class Character {
   private:
    ConsoleGame::Vec2 coord, size = {.width = 32, .height = 32};
    int maxHealth, curHealth;
    GameType::CharaType _type;
    double _speed;
    ConsoleGame::AniSprite* currentSprite;
    ConsoleGame::AniSprite leftSprite, upSprite, rightSprite, downSprite;

   public:
    Character() = default;

    void Init(GameType::CharaType type)
    {
        coord = {.x = 50, .y = 50};
        maxHealth = GameType::CHARA_HEALTH[type];
        _speed = GameType::CHARA_SPEED[type];
        curHealth = maxHealth;
        _type = type;

        std::string_view charNameFile = GameType::CHARA_NAME_FILE[type];
        leftSprite.Load(std::format(
            "{}{}{}L.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
        ));
        rightSprite.Load(std::format(
            "{}{}{}R.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
        ));
        upSprite.Load(std::format(
            "{}{}{}B.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
        ));
        downSprite.Load(std::format(
            "{}{}{}F.anisprite", RESOURCE_PATH, CHARACTER_PATH, charNameFile
        ));

        leftSprite.Play(1);
        rightSprite.Play(1);
        upSprite.Play(1);
        downSprite.Play(1);

        currentSprite = &upSprite;
    };

    void MoveLeft(float deltaTime)
    {
        coord.x -= deltaTime * _speed;
        currentSprite = &leftSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void MoveRight(float deltaTime)
    {
        coord.x += deltaTime * _speed;
        currentSprite = &rightSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
        // currentSprite.AdvanceFrame();
    };

    void MoveUp(float deltaTime)
    {
        coord.y -= deltaTime * _speed;
        currentSprite = &upSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void MoveDown(float deltaTime)
    {
        coord.y += deltaTime * _speed;
        currentSprite = &downSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void Draw(ConsoleGame::AbstractCanvas*& canvas) const
    {
        currentSprite->Paint(canvas, coord);
    }

    void SetCurHealth(int health) { curHealth = health; }

    void SetMaxHealth(int health) { maxHealth = health; }

    void SetSpeed(const double& speed) { _speed = speed; }

    int GetCurHealth() const { return curHealth; }

    int getMaxHealth() const { return maxHealth; }

    int getSpeed() const { return _speed; }
};
