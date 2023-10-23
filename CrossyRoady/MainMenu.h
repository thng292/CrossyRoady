#pragma once
#include "ConsoleGame.h"

class MainMenu : public ConsoleGame::AbstractScreen {
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    AbstractScreen* Clone() const override;
    AbstractNavigation::NavigationRes Update(
        float deltaTime, const AbstractNavigation* navigation
    ) override;
    void Draw(AbstractCanvas* canvas) const override;
};
