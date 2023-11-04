#pragma once

#include <format>
#include <string_view>

#include "ConsoleGame.h"
#include "GameType.h"

class Character {
   private:
    ConsoleGame::Vec2 coord, size = {.width = 32, .height = 32};
    int maxHealth, curHealth;
    GameType::CharaType _type;
    double _speed;
    ConsoleGame::AniSprite currentSprite;
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
        leftSprite.Load(std::format("{}-left.anisprite", charNameFile));
        rightSprite.Load(std::format("{}-right.anisprite", charNameFile));
        upSprite.Load(std::format("{}-up.anisprite", charNameFile));
        downSprite.Load(std::format("{}-down.anisprite", charNameFile));

        currentSprite = upSprite;
    };

    void MoveLeft(const double& distance)
    {
        coord.x -= distance;
        currentSprite = leftSprite;
    };

    void MoveRight(const double& distance)
    {
        coord.x += distance;
        currentSprite = leftSprite;
    };

    void MoveUp(const double& distance)
    {
        coord.y -= distance;
        currentSprite = upSprite;
    };

    void MoveDown(const double& distance)
    {
        coord.y += distance;
        currentSprite = downSprite;
    };

    void SetCurHealth(int health) { curHealth = health; }

    void SetMaxHealth(int health) { maxHealth = health; }

    void SetSpeed(const double& speed) { _speed = speed; }

    int GetCurHealth() const { return curHealth; }

    int getMaxHealth() const { return maxHealth; }

    int getSpeed() const { return _speed; }
};
