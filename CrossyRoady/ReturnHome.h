#pragma once
#include "ConsoleGame.h"
#include "MainMenu.h"
#include "Menu.h"
#include "SharedAudio.h"
#include "StringRes.h"

class ReturnHome : public ConsoleGame::AbstractScreen {
    Menu<3> menu;
    SharedAudio& audio = SharedAudio::GetInstance();
    Button title;
    // Inherited via AbstractScreen
    static const std::wstring_view ScreenName();
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual ConsoleGame::AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
