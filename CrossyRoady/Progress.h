#pragma once
#include "ConsoleGame.h"
#include "Menu.h"
#include "MenuBG.h"
#include "SharedAudio.h"
#include "ArrowButton.h"

class Progress : public ConsoleGame::AbstractScreen {
    Menu<1> backButt;
    MenuBG* bg;
    SharedAudio& audio = SharedAudio::GetInstance();
    uint8_t oldSelected = 0;
    std::array<std::string, 8> data;
    std::array<ConsoleGame::Vec2, 3> rectPos;
    std::array<uint8_t, 3> rectR;
    int earnedXP = 0;
    int currentLevel = 10;
    
    ArrowButton leftArr;
    ArrowButton rightArr;

    Surface surfaceStat;
    Surface surfaceExp;
    
    void DrawStat(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawExp(ConsoleGame::AbstractCanvas* canvas) const;

   public:
    Progress();
    static const std::wstring_view ScreenName();
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    void Mount(const std::any& args) override;
    ConsoleGame::AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
    void Unmount() override;
};
