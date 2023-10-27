#pragma once
#include "ConsoleGame.h"
#include "Surface.h"

class MainMenu : public ConsoleGame::AbstractScreen {
    Surface test;
   public:
    MainMenu();
    static const std::wstring_view ScreenName();
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    ConsoleGame::AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
