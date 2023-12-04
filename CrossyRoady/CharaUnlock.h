#pragma once
#include "ConsoleGame.h"
#include "GameType.h"
#include "MainMenu.h"
#include "Menu.h"
#include "SharedAudio.h"
#include "StringRes.h"

class CharaUnlock : public ConsoleGame::AbstractScreen {
    ConsoleGame::Sprite portrait;
    ConsoleGame::Vec2 portraitPos;
    SharedAudio& audio = SharedAudio::GetInstance();
    ConsoleGame::Audio unlockSfx;
    bool redraw = true;
    CharStuff charStuff;
    Menu<1> menu;

    std::string_view charaName;
    GameType::MapType mapType;
    uint16_t yShift = 30;

    void LoadStuff();

   public:
    CharaUnlock() = default;
    static const std::wstring_view ScreenName();
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    ConsoleGame::AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
