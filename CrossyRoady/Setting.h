#pragma once
#include "Button.h"
#include "ConsoleGame.h"
#include "Menu.h"
#include "MenuBG.h"
#include "SharedAudio.h"

class Setting : public ConsoleGame::AbstractScreen {
    static constexpr ConsoleGame::Vec2 buttDim = {150, 18};
    static constexpr ConsoleGame::Vec2 startPos = {(384 - 150) / 2, 70};

    Button title;
    Menu<5> menu;
    std::string MusicTitle;
    std::string SfxTitle;
    MenuBG* bg;
    SharedAudio& audio = SharedAudio::GetInstance();

    void UpdateMusicTitle();
    void UpdateSfxTitle();

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
};
