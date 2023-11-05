#pragma once
#include "ConsoleGame.h"
#include "Button.h"
#include "Menu.h"

class Setting : ConsoleGame::AbstractScreen {
    static constexpr ConsoleGame::Vec2 MenuStartPos = {0, 0};
    static constexpr ConsoleGame::Vec2 ButtDim = {200, 19};

    Button title;
    Menu<4> menu;
    std::string* sfxOpt = nullptr;

   public:
    static const std::wstring_view ScreenName();
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    ConsoleGame::AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
