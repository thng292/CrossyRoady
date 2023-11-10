#pragma once

#include <format>
#include <string_view>

#include "ConsoleGame.h"
#include "GameType.h"
#include "StringRes.h"

class Character {
   private:
    ConsoleGame::Vec2 size = {.width = 24, .height = 32};

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

   public:
    Character() = default;

    void Init(GameType::CharaType type)
    {
        x = 0;
        y = 100;

        maxHealth = GameType::CHARA_HEALTH[type];
        _speed = GameType::CHARA_SPEED[type];
        _speed = 100;
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
        rightSprite.SetFrameDuration(frameDur);
        upSprite.SetFrameDuration(frameDur);
        downSprite.SetFrameDuration(frameDur);

        leftSprite.Play(1);
        rightSprite.Play(1);
        upSprite.Play(1);
        downSprite.Play(1);

        currentSprite = &upSprite;
    };

    void MoveLeft(float deltaTime)
    {
        x -= deltaTime * _speed;
        currentSprite = &leftSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void MoveRight(float deltaTime)
    {
        x += deltaTime * _speed;
        currentSprite = &rightSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void MoveUp(float deltaTime)
    {
        y += deltaTime * _speed;
        currentSprite = &upSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void MoveDown(float deltaTime)
    {
        y -= deltaTime * _speed;
        currentSprite = &downSprite;
        currentSprite->AutoUpdateFrame(deltaTime);
    };

    void Draw(ConsoleGame::AbstractCanvas*& canvas) const
    {
        currentSprite->Paint(canvas, GetDrawCoord());
        ConsoleGame::Color color = ConsoleGame::Color::BLACK;

        /*for (int i = 0; i < size.width; i++) {
            (*canvas)[GetDrawCoord().y][(int)x + i] = color;
            (*canvas)[GetDrawCoord().y + (int)size.height][(int)x + i] = color;
        }
        for (int i = 0; i < size.height; i++) {
            (*canvas)[GetDrawCoord().y + i][(int)x] = color;
            (*canvas)[GetDrawCoord().y + i][(int)x + (int)size.width] = color;
        }*/
    }

    void SetCurHealth(int health) { curHealth = health; }

    void SetMaxHealth(int health) { maxHealth = health; }

    void SetSpeed(const double& speed) { _speed = speed; }

    int GetCurHealth() const { return curHealth; }

    int getMaxHealth() const { return maxHealth; }

    int getSpeed() const { return _speed; }

    float GetBottomY() const { return y - currentSprite->GetDim().height; }

    float GetX() const { return x; }

    float GetY() const { return y; }

    ConsoleGame::Vec2 GetSize() const { return size; }

    ConsoleGame::Vec2 GetDrawCoord() const
    {
        int screenHeight = ConsoleGame::_CONSOLE_HEIGHT_ * 2;
        return {.x = (int)x, .y = screenHeight - (int)y};
    }
};
