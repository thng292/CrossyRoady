#pragma once
#include "ConsoleGame.h"
#include "MenuBG.h"
#include "TabMenu.h"

class Progress : public ConsoleGame::AbstractScreen {
    Button title;
    TabMenu<4> menu;

    ConsoleGame::Audio* bgMusic;
    ConsoleGame::Audio* hoverSfx;
    MenuBG* bg;

    std::string* sfxOpt;
    std::array<std::unique_ptr<ConsoleGame::AbstractScreen>, 2> subScreen;
    uint8_t oldSelected = 0;

   public:
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
