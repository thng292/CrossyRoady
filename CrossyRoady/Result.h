#pragma once
#include "ArrowButton.h"
#include "ConsoleGame.h"
#include "GameType.h"
#include "HMenu.h"
#include "MenuBG.h"
#include "SharedAudio.h"

class Result : public ConsoleGame::AbstractScreen {
    HMenu<2> menu;
    MenuBG* bg;
    SharedAudio& audio = SharedAudio::GetInstance();
    ConsoleGame::Audio resultSfx;

    std::array<std::string, 7> data;
    Surface surfaceStat;
    Surface surfaceExp;

    GameType::GameResult gameRes;
    bool charaUnlock = false;
    bool mapUnlock = false;
    bool levelUp = false;

    int* charReq = ((int*)&R.CharReq);

    void DrawStat(ConsoleGame::AbstractCanvas* canvas) const;
    bool CheckCharaUnlock();

   public:
    Result() = default;
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
