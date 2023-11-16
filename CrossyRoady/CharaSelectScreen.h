#pragma once

#include "ConsoleGame.h"
#include "SharedAudio.h"
#include "Surface.h"
#include "StringRes.h"

class CharacterSelectScreen : public ConsoleGame::AbstractScreen {
    std::array<ConsoleGame::Sprite, numberOfChars> charAvaMenu;
    std::array<Surface, 10> surfaces;
    ConsoleGame::AniSprite charShowCase;
    ConsoleGame::Sprite speedIcon;
    ConsoleGame::Sprite heartIcon;
    ConsoleGame::Palette currentPalette;
    std::string_view charName;
    uint8_t selected = 0;
    uint8_t lastSelected = 0;
    SharedAudio& audio = SharedAudio::GetInstance();

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
    void Unmount() override;
};