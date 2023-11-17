#pragma once

#include "ConsoleGame.h"
#include "SharedAudio.h"
#include "StringRes.h"
#include "Surface.h"

class CharacterSelectScreen : public ConsoleGame::AbstractScreen {
    // clang-format off
    using Vec2 = ConsoleGame::Vec2;
    static constexpr std::array<Vec2, numberOfChars> charAvaPos = {
        Vec2{131, 30}, {197, 30},
        {131, 91}, {197, 91},
        {131, 152}, {197, 152}
    };
    // clang-format on 
    std::array<ConsoleGame::Sprite, numberOfChars> charAvaMenu;
    std::array<Surface, 4> surfaces;
    ConsoleGame::AniSprite charShowCase;
    ConsoleGame::Sprite speedIcon;
    ConsoleGame::Sprite heartIcon;
    ConsoleGame::Sprite skillIcon;
    ConsoleGame::Palette currentPalette;
    std::string_view charName;
    uint8_t selected = 0;
    uint8_t lastSelected = 0;
    SharedAudio& audio = SharedAudio::GetInstance();

    void DrawLeftPanel(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawRightPanel(ConsoleGame::AbstractCanvas* canvas) const;
    void LoadRes(bool fresh);
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