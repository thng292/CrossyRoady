#pragma once
#include <random>
#include <vector>

#include "ConsoleGame.h"
#include "GameType.h"
#include "Lane.h"

class Rail : public Lane {
   private:
    GameType::MobType _type;
    ConsoleGame::AniSprite _mobSprite;

   public:
    Rail() = default;

    void CreateEntity() override
    {
        int space = ConsoleGame::_CONSOLE_WIDTH_ * 2;
        int numOfEntity = 7;
        if (IsLeftToRight) {
            if (entityList.back() > ConsoleGame::_CONSOLE_WIDTH_) {
                int newX = entityList.back() - space;
                for (int i = 0; i < numOfEntity; i++) {
                    entityList.push_back(newX - i * entityWidth - 1);
                }
            }
        } else {
            if (entityList.back() < 0) {
                int newX = entityList.back() + space;
                for (int i = 0; i < numOfEntity; i++) {
                    entityList.push_back(newX + i * entityWidth + 1);
                }
            }
        }
    }

    void Init() override
    {
        int numOfEntity = 7;
        speed = 200;
        if (IsLeftToRight) {
            for (int i = 0; i < numOfEntity; i++) {
                entityList.push_back((-i * entityWidth - 1));
            }
            CreateEntity();
        } else {
            for (int i = 0; i < numOfEntity; i++) {
                entityList.push_back(
                    ConsoleGame::_CONSOLE_WIDTH_ - (-i * entityWidth - 1)
                );
            }
            CreateEntity();
        }
    }

    void UpdatePos(float deltaTime) override
    {
        float speedWithDirec = (IsLeftToRight) ? speed : -speed;
        for (size_t i = 0; i < entityList.size(); i++) {
            entityList[i] += speedWithDirec * deltaTime;
        }
        CreateEntity();
        DeleteEntity();
    }

    Rail(
        float y,
        GameType::MobType type,
        ConsoleGame::Sprite& roadSprite,
        const ConsoleGame::AniSprite& mobSprite,
        bool isLeftToRight
    )
        : Lane(
              y,
              mobSprite.GetDim(),
              roadSprite,
              GameType::LaneType::RAIL,
              isLeftToRight
          )
    {
        _type = type;
        _mobSprite = mobSprite;
        _mobSprite.Play(1);
        Init();
    }

    void UpdateSprite(float deltaTime)
    {
        _mobSprite.AutoUpdateFrame(deltaTime);
    }

    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const override
    {
        size_t listSize = entityList.size();
        for (size_t i = 0; i < listSize; ++i) {
            _mobSprite.Draw(canvas, {(int)entityList[i], entityDrawY});
            // GameUtils::DrawHitbox(canvas, GetHitBox(i));
        }
    }

    GameType::MobType GetMobType() { return _type; }

    ConsoleGame::Box GetHitBox(size_t ind) const
    {
        ConsoleGame::Box hitbox = _mobSprite.GetHitBox();
        hitbox.coord.x += entityList[ind];
        hitbox.coord.y = entityY - hitbox.coord.y;
        return hitbox;
    }
};
