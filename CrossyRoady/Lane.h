#pragma once
#include <vector>

#include "ConsoleGame.h"
#include "GameType.h"

class Lane {
   protected:
    GameType::LaneType _type;
    std::vector<float> entityList;
    bool IsLeftToRight;
    float speed = 120.0f;

    int laneDrawY;
    int entityDrawY;

    float laneY;

    float entityWidth;
    float entityHeight;
    float entityY;
    float entityFeetY;

    ConsoleGame::Sprite _laneSprite;

   public:
    void DeleteEntity()
    {
        {
            if (IsLeftToRight) {
                float maxX = ConsoleGame::_CONSOLE_WIDTH_ - entityWidth + 1;
                if (entityList.front() >= maxX) {
                    entityList.erase(entityList.begin());
                }
            } else {
                float minX = -entityWidth;
                if (entityList.front() <= minX) {
                    entityList.erase(entityList.begin());
                }
            }
        }
    }

    void CreateEntity()
    {
        float tmp = rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2);
        if (IsLeftToRight) {
            entityList.push_back(entityList.back() + (tmp + entityWidth));

        } else {
            entityList.push_back(entityList.back() - tmp - entityWidth);
        }
    }

    void Init()
    {
        if (IsLeftToRight) {
            entityList.push_back(0);
            float maxX = ConsoleGame::_CONSOLE_WIDTH_ - entityWidth + 1;
            while (entityList.back() <= maxX) {
                CreateEntity();
            }
        } else {
            entityList.push_back(ConsoleGame::_CONSOLE_WIDTH_ - 1);
            float minX = entityWidth - 2;
            while (entityList.back() >= minX) {
                CreateEntity();
            }
        }
    }

    void UpdatePos(float deltaTime)
    {
        for (size_t i = 0; i < entityList.size(); i++) {
            entityList[i] += speed * deltaTime;
        }
        CreateEntity();
        DeleteEntity();
    }

    virtual void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const = 0;

    Lane() = default;

    Lane(
        float y,
        float width,
        float height,
        const ConsoleGame::Sprite& laneSprite,
        GameType::LaneType type
    )
    {
        entityWidth = width;
        entityHeight = height;
        SetY(y);
        _laneSprite = laneSprite;
        IsLeftToRight = 1;
        _type = type;
    }

    void DrawLane(ConsoleGame::AbstractCanvas* canvas) const
    {
        for (int x = 0; x < ConsoleGame::_CONSOLE_WIDTH_; x += 32) {
            _laneSprite.Paint(canvas, {x, laneDrawY});
        }
    }

    float GetY() const { return laneY; }

    float GetEntityFeetY() const { return entityFeetY; }

    float GetTopY() const { return entityFeetY + entityHeight; }

    int GetDrawY() const { return laneDrawY; }

    void SetY(float y)
    {
        int screenHeight = ConsoleGame::_CONSOLE_HEIGHT_ * 2;
        laneY = y;
        laneDrawY = screenHeight - laneY;

        switch (_type) {
            case GameType::ROAD:
                entityY = laneY + (entityHeight / 2 - 5);
                break;
            case GameType::WATER:
                entityY = laneY - 1;
                break;
        }

        entityDrawY = screenHeight - entityY;
        entityFeetY = entityY - entityHeight;
    }
};