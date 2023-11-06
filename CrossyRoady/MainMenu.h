#pragma once
#include <array>
#include <memory>

#include "Button.h"
#include "ConsoleGame.h"
#include "Menu.h"
#include "MenuBG.h"
#include "Common.h"

class MainMenu : public ConsoleGame::AbstractScreen {
    static constexpr ConsoleGame::Vec2 buttDim = {100, 18};
    static constexpr ConsoleGame::Vec2 startPos = {(384 - 100) / 2, 90};

    ConsoleGame::Audio bgMusic;
    ConsoleGame::Audio hoverSfx;
    MenuBG bg;
    SharedMenuStuff passArgs;

    bool shouldCloseSharedResource = true;
    Menu<5> menu;
    std::string* sfxOpt;

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
