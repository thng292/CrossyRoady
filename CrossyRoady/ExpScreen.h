#pragma once
#include "ConsoleGame.h"
#include "Surface.h"
#include "StringRes.h"

class ExpScreen : public ConsoleGame::AbstractScreen {
    ConsoleGame::Audio* hoverSfx;
   public:
    static const std::wstring_view ScreenName();
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    ConsoleGame::AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
