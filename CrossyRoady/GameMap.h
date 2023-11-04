#pragma once
#include <functional>
#include <vector>

#include "Character.h"
#include "ConsoleGame.h"
#include "GameType.h"
#include "Road.h"

class GameMap : public ConsoleGame::AbstractScreen {
    Character gameChara;
    std::vector<Road> roadPosList;

    ConsoleGame::Sprite roadSprite;
    ConsoleGame::Sprite blockSprite;
    ConsoleGame::Sprite logSprite;

    GameType::MobSprite mobEasySprite;
    GameType::MobSprite mobNormalSprite;
    GameType::MobSprite mobHardSprite;

    std::function<void()> debuff;

    const std::wstring_view ScreenName();

    // Inherited via AbstractScreen
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual ConsoleGame::AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;

   public:
    void InitRoadPosList();
    void AddRoad();
    void DeleteRoad();

    void SetMapArgs(const GameType::GameMapArgs& mapArgs)
    {
        roadSprite = mapArgs.roadSprite;
        logSprite = mapArgs.logSprite;
        blockSprite = mapArgs.blockSprite;

        mobEasySprite = mapArgs.mobSpriteEasy;
        mobNormalSprite = mapArgs.mobSpriteNormal;
        mobHardSprite = mapArgs.mobSpriteHard;

        gameChara.Init(mapArgs.charaType);
    };
};
