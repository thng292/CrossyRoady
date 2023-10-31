#include <memory>

#include "CharaSelectScreen.h"
#include "ConsoleGame.h"
#include "MainMenu.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    Font::Load("fontsm.font", 1);
    LoadImage(
        NULL,
        L"../resource/extra/arrow.cur",
        IMAGE_CURSOR,
        0,
        0,
        LR_LOADFROMFILE | LR_DEFAULTSIZE
    );
    // LocalStorage::LoadFromFile();
    // defer { LocalStorage::SaveToFile(); };
    auto game =
        std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<MainMenu>());
    game->Run(MainMenu::ScreenName());
    return 0;
}
