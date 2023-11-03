#pragma once
#include <array>

#include "Button.h"
#include "ConsoleGame.h"

class MainMenu : public ConsoleGame::AbstractScreen {
    std::array<Button, 5> buttons;
    ConsoleGame::AniSprite pre;
    ConsoleGame::Palette prep;
    static constexpr int gap = 5;
    static constexpr ConsoleGame::Vec2 buttDim = {100, 18};
    static constexpr ConsoleGame::Vec2 startPos = {
        (384 - 100) / 2, 90};
    uint8_t selected = 0;
    uint8_t currentScr = 0;
    float timePassed = 0;
    static constexpr float buttonDelay = 1.0f/5;
    bool lastIsUp = false;

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
