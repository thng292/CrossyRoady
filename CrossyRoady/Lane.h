#pragma once
#include <deque>

#include "ConsoleGame.h"

class Lane {
   protected:
    int laneY;
    int laneDrawY;
    GameType::LaneType _type;

    std::deque<int> entityList;
    int entityWidth;
    int entityHeight;
    int entityDrawY;
    int entityFeetY;

    bool IsLeftToRight;
    int speed = 120.0f;

   public:
    Lane() = default;

    void CreateEntity()
    {
        int tmp = rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2);
        if (IsLeftToRight) {
            entityList.push_back(entityList.back() + (tmp + entityWidth));

        } else {
            entityList.push_back(entityList.back() - tmp - entityWidth);
        }
    }

    void DeleteEntity()
    {
        if (IsLeftToRight) {
            int maxX = ConsoleGame::_CONSOLE_WIDTH_ - entityWidth + 1;
            if (entityList.front() >= maxX) entityList.pop_front();
        } else {
            int minX = -entityWidth;
            if (entityList.front() <= minX) entityList.pop_front();
        }
    }

    void Init()
    {
        if (IsLeftToRight) {
            entityList.push_back(0);
            int maxX = ConsoleGame::_CONSOLE_WIDTH_ - entityWidth + 1;
            while (entityList.back() <= maxX) {
                CreateEntity();
            }
        } else {
            entityList.push_back(ConsoleGame::_CONSOLE_WIDTH_ - 1);
            int minX = entityWidth - 2;
            while (entityList.back() >= minX) {
                CreateEntity();
            }
        }
    }

    Lane(int y, int width, int height, GameType::LaneType type)
    {
        entityWidth = width;
        entityHeight = height;
        laneY = y;
        laneDrawY = ConsoleGame::_CONSOLE_HEIGHT_ * 2 - laneY;
        IsLeftToRight = 1;
        _type = type;
        Init();
    }

    void UpdatePos(float deltaTime)
    {
        for (size_t i = 0; i < entityList.size(); i++) {
            entityList[i] += speed * deltaTime;
        }
        CreateEntity();
        DeleteEntity();
    }

    int GetY() const { return laneY; }

    int GetEntityFeetY() const { return entityFeetY; }

    int GetDrawY() const { return laneDrawY; }

    virtual void DrawLane(ConsoleGame::AbstractCanvas* canvas) const = 0;
    virtual void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const = 0;
    virtual int GetHeight() const = 0;
};