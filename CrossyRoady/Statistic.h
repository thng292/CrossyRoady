#pragma once
#include "Button.h"
#include "ConsoleGame.h"
#include "Surface.h"
#include "Menu.h"
#include "MenuBG.h"

class Statistic : public ConsoleGame::AbstractScreen {
    Button titleButt;
    Surface surface;

    ConsoleGame::Audio* hoverSfx;

    std::array<std::string, 5> data;

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
