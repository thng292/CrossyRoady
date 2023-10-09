#pragma once
#include <any>
#include <string_view>

#include "ConsoleGame.h"

class DevScreen : public ConsoleGame::AbstractScreen {
    int moveX = 0;
    int moveY = 0;

    const ConsoleGame::Vec2 dim{.width = 3, .height = 3};

    // Inherited via AbstractScreen
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime,
        const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
    void DeInit() override;
};
