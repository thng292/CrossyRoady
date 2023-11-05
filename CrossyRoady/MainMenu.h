#pragma once
#include <array>

#include "ConsoleGame.h"
#include "Menu.h"
#include "SubMenuCommon.h"

class MainMenu: public AbstractSubMenu {
    static constexpr ConsoleGame::Vec2 buttDim = {100, 18};
    static constexpr ConsoleGame::Vec2 startPos = {(384 - 100) / 2, 90};

    Menu<5> menu;
    std::string* sfxOpt = nullptr;

   public:
    void Init() override;
    void Mount() override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation, SubMenu& currentScreen
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
    void Unmount() override;
};
