#pragma once
#include <fstream>

#include "CharaSelectScreen.h"
#include "ConsoleGame.h"
#include "GameMap.h"
#include "Menu.h"
#include "SharedAudio.h"
#include "StringRes.h"

// #include "MapSele"

class AskSave : public ConsoleGame::AbstractScreen {
    Menu<3> menu;
    MenuBG* bg = nullptr;
    ConsoleGame::Vec2 drawPos;
    SharedAudio& audio = SharedAudio::GetInstance();
    Surface surface;
    Surface surfaceData;
    SaveData saveData;

    std::array<std::string, 6> data;
    Character* chara = ((Character*)&R.String.Character);
    MapStuff* mapStuff = ((MapStuff*)&R.String.Map);
    std::time_t currentTime;

    void DataPeak();
    void DrawStat(ConsoleGame::AbstractCanvas* canvas) const;

   public:
    // Inherited via AbstractScreen
    static const std::wstring_view ScreenName();
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual ConsoleGame::AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};