#pragma once
#include "ArrowButton.h"
#include "ConsoleGame.h"
#include "Menu.h"
#include "GameType.h"

class MapSelect : public ConsoleGame::AbstractScreen {
    ConsoleGame::Sprite preview;
    uint8_t selectedMap = 0;
    uint8_t selectedMusic = 0;
    uint8_t selectedDifficulty = 0;
    uint8_t selectedMode = 0;
    bool enableDebuff = 0;

    std::string MusicTitle;
    std::string DifficultyTitle;
    std::string ModeTitle;
    std::string DebuffTitle;

    Menu<5> menu;
    ArrowButton mapL, mapR;
    ArrowButton difficultyL, difficultyR;
    ArrowButton modeL, modeR;
    ArrowButton musicL, musicR;

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
