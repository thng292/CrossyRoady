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
    float _speed, x = 0, y = 50;
    ConsoleGame::AniSprite* currentSprite;
    ConsoleGame::AniSprite leftSprite, upSprite, rightSprite, downSprite;
    int inpR = 0, inpL = 0, distR = 0, distL = 0, timeL = 0, timeR = 0;

   public:
    Character() = default;

    void Init(GameType::CharaType type)
    {
        coord = {.x = 0, .y = 50};
        maxHealth = GameType::CHARA_HEALTH[type];
        _speed = GameType::CHARA_SPEED[type];
        _speed = 120;
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

        float frameDur = 0.15f;
        leftSprite.SetFrameDuration(frameDur);
        leftSprite.SetFrameDuration(frameDur);
        leftSprite.SetFrameDuration(frameDur);
        leftSprite.SetFrameDuration(frameDur);

        leftSprite.Play(1);
        rightSprite.Play(1);
        upSprite.Play(1);
        downSprite.Play(1);

        currentSprite = &upSprite;
    };

    void MoveLeft(float deltaTime)
    {
        float tmp = deltaTime * _speed;
        distL += tmp;
        timeL += deltaTime;
        /* LogDebug(
             "[Left] Dist: {}, Total: {}, Deltatime: {}, Pressed: {}, X: {}",
             tmp,
             distL,
             deltaTime,
             ++inpL,
             coord.x
         );*/
        coord.x -= deltaTime * _speed;
        currentSprite = &leftSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void MoveRight(float deltaTime)
    {
        float tmp = deltaTime * _speed;
        distR += tmp;
        timeR += deltaTime;

        /*LogDebug(
            "[Right] Dist: {}, Total: {}, Deltatime: {}, Pressed: {}, X: {}",
            tmp,
            distR,
            deltaTime,
            ++inpR,
            coord.x
        );*/

        coord.x += deltaTime * _speed;
        currentSprite = &rightSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
        // currentSprite.AdvanceFrame();
    };

    void MoveUp(float deltaTime)
    {
        coord.y += deltaTime * _speed;
        currentSprite = &upSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void MoveDown(float deltaTime)
    {
        coord.y -= deltaTime * _speed;
        currentSprite = &downSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void Draw(ConsoleGame::AbstractCanvas*& canvas) const
    {
        currentSprite->Paint(canvas, GetDrawCoord());
    }

    void SetCurHealth(int health) { curHealth = health; }

    void SetMaxHealth(int health) { maxHealth = health; }

    void SetSpeed(const double& speed) { _speed = speed; }

    int GetCurHealth() const { return curHealth; }

    int getMaxHealth() const { return maxHealth; }

    int getSpeed() const { return _speed; }

    ConsoleGame::Vec2 GetCoord() const { return coord; }

    ConsoleGame::Vec2 GetCoordFeet() const
    {
        ConsoleGame::Vec2 feetCoord{
            .x = coord.x, .y = coord.y - currentSprite->GetDim().height};
        return feetCoord;
    }

    ConsoleGame::Vec2 GetDrawCoord() const
    {
        ConsoleGame::Vec2 drawCoord{
            .x = coord.x, .y = ConsoleGame::_CONSOLE_HEIGHT_ * 2 - coord.y};
        return drawCoord;
    }
};
