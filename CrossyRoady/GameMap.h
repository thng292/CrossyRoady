#pragma once
#include <functional>
#include <memory>
#include <queue>
#include <vector>

#include "Character.h"
#include "ConsoleGame.h"
#include "GameType.h"
#include "GameUtils.h"
#include "Lane.h"
#include "Road.h"
#include "SafeZone.h"
#include "Water.h"

using Debuff = std::function<void()>;

class GameMap : public ConsoleGame::AbstractScreen {
    std::vector<std::unique_ptr<Lane>> laneList;

    std::queue<Debuff> debuffQueue;

    GameType::GameMapData gameData;
    GameType::GameMapSprites gameSprites;
    Character character;

    GameType::GameFlags gameFlags;

    int tmpCol;

    float mapSpeed = 10.0f;

    // Inherited via AbstractScreen
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual ConsoleGame::AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;

    virtual void Mount(const std::any& args) override;
    void Unmount() override;

   public:
    GameMap() = default;

    static const std::wstring_view ScreenName();
    void InitRoadPosList();
    void AddRoad();
    void DeleteRoad();
    void SetGameMapData(const GameType::GameMapData& gmData);

    void DragMapDown(float deltatime);

    void DrawFlat(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const;

    void DrawHealth(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawSkill(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawDebuff(ConsoleGame::AbstractCanvas* canvas) const;

    void ResetFlags();
    void CollisionCheck();
    void HandleCollision(
        const std::unique_ptr<Lane>& lane, GameType::CollisionType colType
    );
    void HandleWaterCollision(GameType::CollisionType colType);

    void HandlePlayerInput();
    void HandlePlayerAnimation(float deltaTime);
    void HandlePlayerMovement(float deltaTime);
};
