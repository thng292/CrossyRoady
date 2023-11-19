#pragma once
#include <vector>

#include "Character.h"
#include "ConsoleGame.h"
#include "GameType.h"

class Lane {
   protected:
    GameType::LaneType _type;
    std::vector<float> entityList;
    bool IsLeftToRight;
    bool _hasItem = false;
    float speed = 50.0f;

    float laneY;
    int laneDrawY;

    float entityWidth;
    float entityHeight;
    float entityY;
    float entityFeetY;
    int entityDrawY;

    ConsoleGame::Sprite _laneSprite;

   public:
    Lane() = default;
    Lane(
        float y,
        ConsoleGame::Vec2 dim,
        const ConsoleGame::Sprite& laneSprite,
        GameType::LaneType type,
        bool isLeftToRight
    );

    Lane(
        float y,
        ConsoleGame::Vec2 dim,
        GameType::LaneType type,
        bool isLeftToRight,
        bool hasItem,
        const std::vector<float>& enList
    );

    virtual void Init();
    virtual void DeleteEntity();
    virtual void CreateEntity();
    virtual void UpdatePos(float deltaTime);
    bool ContainsChara(const Character& character);

    float GetY() const;
    float GetBottomY() const;
    int GetDrawY() const;
    float GetTopY() const;
    GameType::LaneType GetType() const;
    bool GetIsLeftToRight() const;
    float GetSpeed() const;
    bool GetHasItem() const;

    void SetY(float y);
    void SetHasItem(bool hasItem);
    void SetLaneSprite(const ConsoleGame::Sprite& laneSprite);

    void DrawLane(ConsoleGame::AbstractCanvas* canvas) const;

    GameType::CollisionType GetCollision(const Character& character) const;
    GameType::CollisionType GetLaneCollision(const Character& character) const;
    std::vector<ConsoleGame::Box> GetLaneHitBoxLTR() const;
    std::vector<ConsoleGame::Box> GetLaneHitBoxRTL() const;

    virtual ConsoleGame::Box GetHitBox(size_t index) const = 0;
    virtual void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const = 0;
};