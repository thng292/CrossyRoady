#pragma once

#include "ConsoleGame.h"
#include "SharedAudio.h"

class CharacterSelectScreen : public ConsoleGame::AbstractScreen {
    std::array<ConsoleGame::Sprite, 6> charAvaMenu;
    ConsoleGame::AniSprite charShowCase;
    ConsoleGame::Sprite speedIcon;
    ConsoleGame::Sprite heartIcon;

    uint8_t selected = 0;
    uint8_t lastSelected = 0;
    uint8_t unlocked;
    float mouseCounter = 0;
    float keyCounter = 0;
    static constexpr auto BGPrimary = (ConsoleGame::Color)12;
    static constexpr auto BGSecond = (ConsoleGame::Color)13;

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