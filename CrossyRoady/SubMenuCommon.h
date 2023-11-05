#pragma once
#include "ConsoleGame.h"

enum class SubMenu : uint8_t {
    MainMenu,
    Setting,
    HowToPlay,
    Exp,
    Statistic,
};

class AbstractSubMenu {
   protected:
    ConsoleGame::Audio* hoverSfx;

   public:
    void SetHoverSfx(ConsoleGame::Audio* hoverSfx)
    {
        this->hoverSfx = hoverSfx;
    }

    virtual void Init() = 0;
    virtual void Mount(){};
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime,
        const ConsoleGame::AbstractNavigation* navigation,
        SubMenu& currentScreen
    ) = 0;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const = 0;
    virtual void Unmount(){};
};