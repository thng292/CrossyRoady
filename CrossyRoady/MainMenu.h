#pragma once
#include <array>
#include <memory>

#include "Button.h"
#include "Common.h"
#include "ConsoleGame.h"
#include "Menu.h"
#include "MenuBG.h"
#include "SharedAudio.h"

class MainMenu : public ConsoleGame::AbstractScreen {
    static constexpr ConsoleGame::Vec2 buttDim = {100, 18};
    static constexpr ConsoleGame::Vec2 startPos = {(384 - 100) / 2, 70};

    MenuBG bg;
    SharedAudio& audio = SharedAudio::GetInstance();
    Menu<6> menu;

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
