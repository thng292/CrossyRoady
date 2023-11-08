#pragma once
#include <deque>
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

using Debuff = std::function<void()>;

class GameMap : public ConsoleGame::AbstractScreen {
    std::deque<std::unique_ptr<Lane>> laneList;
    std::queue<Debuff> debuffQueue;

    GameType::GameMapData gameData;
    GameType::GameMapSprites gameSprites;
    Character character;

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
    void HandlePlayerInput(float deltaTime);
    void SetGameMapData(const GameType::GameMapData& gmData);

    void DrawFlat(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const;

    void DrawHealth(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawSkill(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawDebuff(ConsoleGame::AbstractCanvas* canvas) const;

    void DragMapDown(float deltatime)
    {
        // auto laneListEnd = laneList.end();
        /*for (auto it = laneList.begin(); it != laneListEnd; ++it) {
            auto lane = it->get();
            lane->SetY(lane->GetY() - deltatime * mapSpeed);
            lane->GetY();
        }*/
        LogDebug("{}", laneList.size());
        try {
            for (size_t i = 0; i < laneList.size(); ++i) {
                laneList[i]->SetY(laneList[i]->GetY() - deltatime * mapSpeed);
                laneList[i]->GetY();
            }
            if (!laneList.empty() && laneList[0]->GetY() < 32) {
                // laneList.erase(laneList.begin());
                laneList.pop_front();
                laneList.push_back(std::make_unique<Road>(
                    laneList.back()->GetY() + 32,
                    32,
                    32,
                    GameType::MobType::EASY,
                    gameSprites.roadSprite,
                    gameSprites.mobSpriteEasy.MobRight
                ));
                LogDebug("{}", laneList.size());
            }
        } catch (std::exception& e) {
            LogDebug("size {}", laneList.size());
        }
    };
};
