#pragma once
#include "ConsoleGame.h"
#include <any>
#include <string_view>

class DevScreen : public ConsoleGame::AbstractScreen {
    int move = 0;
    // Inherited via AbstractScreen
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        const ConsoleGame::InputRecords& inputs,
        float deltaTime,
        const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
    void DeInit() override;
};
