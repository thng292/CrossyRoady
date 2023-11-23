#pragma once
#include <functional>
#include <memory>
#include <queue>
#include <vector>

#include "Character.h"
#include "ConsoleGame.h"
#include "Control.h"
#include "Credit.h"
#include "GameMaster.h"
#include "GameType.h"
#include "GameUtils.h"
#include "HowToPlay.h"
#include "Item.h"
#include "Lane.h"
#include "MainMenu.h"
#include "Menu.h"
#include "Rail.h"
#include "Result.h"
#include "ReturnHome.h"
#include "Road.h"
#include "SafeZone.h"
#include "SaveLoadType.h"
#include "Setting.h"
#include "SharedAudio.h"
#include "StringRes.h"
#include "Water.h"

class GameMap : public ConsoleGame::AbstractScreen {
    std::vector<std::unique_ptr<Lane>> laneList;  // save
    Character character;                          // save
    Item mapItem;                                 // save
    Lane* laneWithItem;                           // save

    GameType::GameMapData gameData;            // save
    GameMaster::GameEventsArgs gameEventArgs;  // save
    GameMaster::GameFlags gameFlags;           // save

    float gameOverWait = 3.0f;
    GameType::GameMapSprites gameSprites;
    GameType::GameAudio gameAudio;

    SharedAudio& audio = SharedAudio::GetInstance();
    std::array<std::unique_ptr<ConsoleGame::AbstractScreen>, 4> subScreen;
    Menu<3> menu;
    int selectedScr = -1;

   public:
    GameMap() = default;
    static const std::wstring_view ScreenName();
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual ConsoleGame::AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;

    virtual void Mount(const std::any& args) override;
    void Unmount() override;

    // Game stuff
    void SetGameMapData(const GameType::GameMapData& gmData);
    void InitLaneList();
    void ResiseBlockHitBox();

    void LoadSprites();
    void LoadAudio();
    void UnloadSprites();
    void UnloadAudio();

    void DragMapDown(float deltatime);
    void DrawFlat(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawHealth(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawSkill(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawDebuff(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawDarkness(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawScore(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawDeathVFX(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawTime(ConsoleGame::AbstractCanvas* canvas) const;

    void ResetFlags();

    void CheckCollision(float deltaTime);
    void CheckDebuff();
    void CheckSkill();
    void CheckOutOfBound();
    void CheckGameOver();

    void UpdateLanes(float deltaTime);
    void UpdateCooldowns(float deltaTime);
    void UpdateDifficulty();
    void UpdateMapSpeed();
    void UpdateTime(float deltaTime);

    void HandleCollision(
        const std::unique_ptr<Lane>& lane, GameType::CollisionType colType
    );
    void HandleWaterCollision(GameType::CollisionType colType);
    void HandleCharaOnLog(const std::unique_ptr<Lane>& lane, float deltaTime);
    void HandleDamage();
    void HandleDebuff(float deltaTime);
    void HandleSkill(float deltaTime);
    void HandlePlayerInput();
    void HandlePlayerAnimation(float deltaTime);
    void HandlePlayerMovement(float deltaTime);
    bool HandleCharaDraw(
        ConsoleGame::AbstractCanvas* canvas,
        float charBottomY,
        float laneBottomY
    ) const;
    void HandleItemCollision();
    void HandleGameOver(
        float deltaTime,
        ConsoleGame::AbstractNavigation::NavigationRes& res,
        const ConsoleGame::AbstractNavigation* navigation
    );
    void HandleGamePause(
        float deltaTime,
        ConsoleGame::AbstractNavigation::NavigationRes& res,
        const ConsoleGame::AbstractNavigation* navigation
    );

    void TurnOffDebuff();
    void TurnOffSkill();

    void SaveGameData();
    void LoadGameData();

    std::unique_ptr<Lane> GetEquivLane(
        const ValLane& valLane, const std::vector<float>& enList
    );

    std::unique_ptr<Lane> GetRandomLane();
    ConsoleGame::AniSprite GetMobSprite(
        GameType::MobType type, bool isLeftToRight
    );
    ConsoleGame::AniSprite GetItemSprite(GameType::ItemType type);
};
