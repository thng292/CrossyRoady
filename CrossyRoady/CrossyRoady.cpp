#include <memory>

#include "AniScreen.h"
#include "CharaSelectScreen.h"
#include "ConsoleGame.h"
#include "DevScreen.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    // LocalStorage::LoadFromFile();
    // defer { LocalStorage::SaveToFile(); };
    auto game =
        std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
    // auto game = std::make_unique<Game>(100'000);
    // game->AddScreen(std::make_unique<AniScreen>());
    // game->Run(AniScreen::ScreenName());
    game->AddScreen(std::make_unique<CharaSelectScreen>());
    game->Run(CharaSelectScreen::ScreenName());
    return 0;
}
