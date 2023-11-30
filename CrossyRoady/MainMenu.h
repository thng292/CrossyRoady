#pragma once
#include <array>
#include <filesystem>
#include <format>
#include <memory>

#include "Button.h"
#include "Common.h"
#include "ConsoleGame.h"
#include "GameType.h"
#include "GameUtils.h"
#include "Menu.h"
#include "MenuBG.h"
#include "SharedAudio.h"

class MainMenu : public ConsoleGame::AbstractScreen {
    static constexpr ConsoleGame::Vec2 buttDim = {100, 18};
    static constexpr ConsoleGame::Vec2 startPos = {(384 - 100) / 2, 90};

    MenuBG bg;
    SharedAudio& audio = SharedAudio::GetInstance();
    Menu<5> menu;
    ConsoleGame::Sprite title;
    GameType::MapType bgType;

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
