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
    double speed;
    ConsoleGame::AniSprite& currentSprite;
    ConsoleGame::AniSprite leftSprite, upSprite, rightSprite, downSprite;

   public:
    Character() = default;

    void Init(GameType::CharaType type)
    {
        coord = {.x = 50, .y = 50};
        maxHealth = GameType::CHARA_HEALTH[type];
        speed = GameType::CHARA_SPEED[type];
        curHealth = maxHealth;
        _type = type;

        std::string_view charNameFile = GameType::CHARA_NAME_FILE[type];
        leftSprite.Load(std::format("{}-left.anisprite", charNameFile));
        rightSprite.Load(std::format("{}-right.anisprite", charNameFile));
        upSprite.Load(std::format("{}-up.anisprite", charNameFile));
        downSprite.Load(std::format("{}-down.anisprite", charNameFile));

        currentSprite = upSprite;
    };

    void MoveLeft()
    {
        coord.x -= speed;
        currentSprite = leftSprite;
    };
};
