#include <memory>

#include "ConsoleGame.h"
#include "MainMenu.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    Palette pal("test.palette");
    ChangeColorPalette(pal);
    // LocalStorage::LoadFromFile();
    // defer { LocalStorage::SaveToFile(); };
    auto game = std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<MainMenu>());
    game->Run(MainMenu::ScreenName());
    return 0;
}
