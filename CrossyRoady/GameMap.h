#pragma once
#include <functional>
#include <queue>
#include <vector>

#include "Character.h"
#include "ConsoleGame.h"
#include "GameType.h"
#include "GameUtils.h"
#include "Road.h"

using Debuff = std::function<void()>;

class GameMap : public ConsoleGame::AbstractScreen {
    std::vector<Road> roadPosList;
    std::queue<Debuff> debuffQueue;
    GameType::GameMapData gameData;
    GameType::GameMapSprites gameSprites;
    Character character;

    // Inherited via AbstractScreen
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual ConsoleGame::AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;

   public:
    GameMap() = default;

    GameMap(const GameType::GameMapData& gameData);

    static const std::wstring_view ScreenName();
    void InitRoadPosList();
    void AddRoad();
    void DeleteRoad();
    void HandlePlayerInput(float deltaTime);
};
