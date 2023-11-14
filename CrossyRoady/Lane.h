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
    float speed = 10.0f;

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
        ConsoleGame::Box hitbox,
        const ConsoleGame::Sprite& laneSprite,
        GameType::LaneType type
    );

    void Init();
    void DeleteEntity();
    void CreateEntity();
    void UpdatePos(float deltaTime);
    bool ContainsChara(const Character& character);

    float GetY() const;
    float GetBottomY() const;
    int GetDrawY() const;
    float GetTopY() const;
    GameType::LaneType GetType() const;
    bool GetIsLeftToRight() const;
    float GetSpeed() const;

    void SetY(float y);

    void DrawLane(ConsoleGame::AbstractCanvas* canvas) const;

    GameType::CollisionType GetCollision(const Character& character) const;
    GameType::CollisionType GetLaneCollision(const Character& character) const;
    std::vector<ConsoleGame::Box> GetLaneHitBoxLTR() const;
    std::vector<ConsoleGame::Box> GetLaneHitBoxRTL() const;

    virtual ConsoleGame::Box GetHitBox(size_t index) const = 0;
    virtual void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const = 0;
};