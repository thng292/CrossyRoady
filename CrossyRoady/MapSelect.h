#pragma once
#include "ArrowButton.h"
#include "ConsoleGame.h"
#include "GameType.h"
#include "Menu.h"
#include "SharedAudio.h"
#include "StringRes.h"

class MapSelect : public ConsoleGame::AbstractScreen {
    ConsoleGame::Sprite preview;
    ConsoleGame::Sprite debuff;
    uint8_t lastSelectedMap = 0;
    uint8_t selectedMode = 0;
    bool enableDebuff = true;
    bool hasChangedMusic = false;

    std::string MusicTitle;
    std::string DifficultyTitle;
    std::string ModeTitle;
    std::string DebuffTitle;

    Menu<6> menu;
    Menu<SongName.size()> musicMenu;
    Menu<R.String.MapSelect.Modes.size()> modeMenu;
    Menu<R.String.MapSelect.Difficulties.size()> diffMenu;
    Button mapTitleButton;
    Surface debuffSurface;
    uint8_t currentMenu = 0;
    ArrowButton mapL, mapR;
    ConsoleGame::Palette currentPalette;
    SharedAudio& audio = SharedAudio::GetInstance();

    GameType::UserOption userOpt;

    void ChangePreview();
    void UpdateStr();

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
