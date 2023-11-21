#pragma once
#include "ArrowButton.h"
#include "ConsoleGame.h"
#include "HMenu.h"
#include "MenuBG.h"
#include "SharedAudio.h"

class Result : public ConsoleGame::AbstractScreen {
    HMenu<2> menu;
    MenuBG* bg;
    SharedAudio& audio = SharedAudio::GetInstance();
    std::array<std::string, 6> data;

    Surface surfaceStat;
    Surface surfaceExp;

    void DrawStat(ConsoleGame::AbstractCanvas* canvas) const;

   public:
    Result() = default;
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
