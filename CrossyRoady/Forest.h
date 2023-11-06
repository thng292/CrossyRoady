#pragma once
#include "Character.h"
#include "ConsoleGame.h"

class Forest : public ConsoleGame::AbstractScreen {
    ConsoleGame::Palette palette;
    ConsoleGame::Sprite backgroundSprite;
    const std::string_view backgroundSpritePath = "";
    const std::string_view palettePath = "";
    Character character;
    // map data (sprite paths) => reads map data
    //

   public:
    static const std::wstring_view ScreenName();

    // Inherited via AbstractScreen
    std::wstring_view getName() override;
    void init(const std::any& args) override;
    AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
