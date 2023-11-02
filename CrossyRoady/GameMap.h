#pragma once
#include <vector>

#include "Character.h"
#include "ConsoleGame.h"
#include "Road.h"

class GameMap : public ConsoleGame::AbstractScreen {
    Character gameChara;
    std::vector<int> roadPosList, trainPosList, waterPosList;

  

    const std::wstring_view ScreenName();
    void InitRoadPosList();
    void AddRoad();
    void DeleteRoad();

    // Inherited via AbstractScreen
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual ConsoleGame::AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;

   protected:
    Sprite asds;
    virtual void Skill() const = 0;

};
