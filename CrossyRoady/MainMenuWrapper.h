#pragma once
#include "ConsoleGame.h"
#include "SubMenuCommon.h"
#include "MainMenu.h"

class MainMenuWrapper : public ConsoleGame::AbstractScreen {
    ConsoleGame::Audio bgMusic;
    ConsoleGame::Audio hoverSfx;

    SubMenu currentScr = SubMenu::MainMenu;
    std::array<AbstractSubMenu*, 1> SubMenu;
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
