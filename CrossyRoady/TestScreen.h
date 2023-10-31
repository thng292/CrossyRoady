#pragma once
#include "ConsoleGame.h"
#include "Road.h"

class TestScreen : public ConsoleGame::AbstractScreen {
    Road road;
    ConsoleGame::Vec2 poss = {.x = 0, .y = 30},
                      size = {.width = 32, .height = 32};
    // Inherited via AbstractScreen
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

// class MainMenu : public ConsoleGame::AbstractScreen {
//    public:
//     static const std::wstring_view ScreenName();
//     std::wstring_view getName() override;
//     void Init(const std::any& args) override;
//     ConsoleGame::AbstractScreen* Clone() const override;
//     ConsoleGame::AbstractNavigation::NavigationRes Update(
//         float deltaTime, const ConsoleGame::AbstractNavigation* navigation
//     ) override;
//     void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
// };
