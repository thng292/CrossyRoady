#include <memory>

#include "CharaSelectScreen.h"
#include "ConsoleGame.h"
#include "MainMenu.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    // LocalStorage::LoadFromFile();
    // defer { LocalStorage::SaveToFile(); };
    auto game =
        std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<CharaSelectScreen>());
    game->Run(CharaSelectScreen::ScreenName());
    return 0;
}
