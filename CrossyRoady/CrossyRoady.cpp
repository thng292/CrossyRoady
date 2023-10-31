#include <memory>

#include "CharaSelectScreen.h"
#include "ConsoleGame.h"
#include "MainMenu.h"
#include "TestScreen.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    Font::Load("fontsm.font", 1);
    // LocalStorage::LoadFromFile();
    // defer { LocalStorage::SaveToFile(); };
    auto game =
        std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<TestScreen>());
    game->Run(TestScreen::ScreenName());
    return 0;
}
