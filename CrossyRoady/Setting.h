#pragma once
#include "ConsoleGame.h"
#include "Button.h"

class Setting : ConsoleGame::AbstractScreen {
    Button title;
    std::array<Button, 3> buttons;
    static constexpr int gap = 5;
    static constexpr ConsoleGame::Vec2 buttDim = {150, 18};
    static constexpr ConsoleGame::Vec2 startPos = {(384 - 150) / 2, 90};
    uint8_t selected = 0;
    float timePassed = 0;
    static constexpr float buttonDelay = 1.0f / 5;
    bool lastIsUp = false;

   public:
    Setting();
    static const std::wstring_view ScreenName();
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    ConsoleGame::AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
