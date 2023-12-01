#pragma once
#include <memory>

#include "ConsoleGame.h"
#include "GameType.h"
#include "GameUtils.h"
#include "Lane.h"
#include "Rail.h"
#include "Road.h"
#include "SafeZone.h"
#include "Water.h"

class MenuBG {
    std::vector<std::unique_ptr<Lane>> laneList;
    GameType::MapType _type;
    GameType::GameMapSprites sprite;
    bool Unmounted = true;
    uint8_t mountCnt = 0;

    void InitLaneList();
    void LoadSprites();
    void LoadLanes();
    std::unique_ptr<Lane> MakeLane(
        GameType::LaneType type,
        bool isLeftToRight,
        GameType::MobType mobType = GameType::EASY
    );
    ConsoleGame::AniSprite* GetMobSprite(
        GameType::MobType type, bool isLeftToRight
    );
    void DrawFlat(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const;
    void UpdateSprites(float deltaTime);
    void UpdateLane(float deltaTime);

   public:
    bool IsUnmounted();
    void Init(GameType::MapType type);
    void Mount();
    void Update(float deltaTime);
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
    void Unmount();
    void SetUnmount(bool val);
};
