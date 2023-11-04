#pragma once
#include <array>

#include "Button.h"
#include "ConsoleGame.h"

class MainMenu : public ConsoleGame::AbstractScreen {
    static constexpr int gap = 5;
    static constexpr ConsoleGame::Vec2 buttDim = {100, 18};
    static constexpr ConsoleGame::Vec2 startPos = {(384 - 100) / 2, 90};
    static constexpr float buttonDelay = 1.0f / 5;

    ConsoleGame::Palette prep;
    ConsoleGame::Audio bgMusic;
    ConsoleGame::Audio hoverSfx;

    std::array<Button, 5> buttons;
    uint8_t selected = 0;
    uint8_t lastSelected = 0;
    uint8_t currentScr = 0;
    float timePassed = 0;
    bool lastIsUp = false;

    std::string* sfxOpt = nullptr; 

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
