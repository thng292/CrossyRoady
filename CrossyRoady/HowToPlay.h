#pragma once
#include "ConsoleGame.h"
#include "MenuBG.h"
#include "TabMenu.h"
#include "SharedAudio.h"

class HowToPlay : public ConsoleGame::AbstractScreen {
    Surface surface;
    Button Title;
    TabMenu<4> menu;
    MenuBG* bg;
    SharedAudio& audio = SharedAudio::GetInstance();
    std::array<std::unique_ptr<ConsoleGame::AbstractScreen>, 3> subScreen;

   public:
    static const std::wstring_view ScreenName();
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual AbstractScreen* Clone() const override;
    // void Mount(const std::any& args) override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
    // void Unmount() override;
};