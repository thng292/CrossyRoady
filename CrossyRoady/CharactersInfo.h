#pragma once
#include "ArrowButton.h"
#include "ConsoleGame.h"
#include "HMenu.h"
#include "SharedAudio.h"

class CharactersInfo : public ConsoleGame::AbstractScreen {
    ConsoleGame::Sprite portrait;
    ConsoleGame::Vec2 portraitPos;
    SharedAudio& audio = SharedAudio::GetInstance();
    ArrowButton leftArr;
    ArrowButton rightArr;
    uint8_t currentSelect = 0;
    HMenu<2> backButt;

    void LoadStuff();

   public:
    CharactersInfo();
    static const std::wstring_view ScreenName();
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    ConsoleGame::AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};
