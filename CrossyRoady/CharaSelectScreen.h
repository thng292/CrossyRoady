#pragma once
#include "ConsoleGame.h"

class CharaSelectScreen : public ConsoleGame::AbstractScreen {
    ConsoleGame::Palette palette;
    const std::string_view fileName = "../spritegen/chara-select.sprite";
    const std::string_view paletteName = "../spritegen/chara-select.palette";
    ConsoleGame::Sprite bg;
    const ConsoleGame::Vec2 DRAW_COORD{.x = 0, .y = 0};

   public:
    static const std::wstring_view ScreenName();

    // Inherited via AbstractScreen
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
