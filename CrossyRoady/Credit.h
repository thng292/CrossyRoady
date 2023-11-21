#pragma once

#include "Button.h"
#include "ConsoleGame.h"
#include "Menu.h"
#include "MenuBG.h"
#include "SharedAudio.h"
#include "Surface.h"

class Credit : public ConsoleGame::AbstractScreen {
    Menu<1> backButt;
    Button buttonTitle;
    Surface surface;
    MenuBG* bg = nullptr;
    SharedAudio& audio = SharedAudio::GetInstance();

   public:
    static const std::wstring_view ScreenName();
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
