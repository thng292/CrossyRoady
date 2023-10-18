#include <memory>

#include "AniScreen.h"
#include "ConsoleGame.h"
#include "DevScreen.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    // LocalStorage::LoadFromFile();
    // defer { LocalStorage::SaveToFile(); };
    //auto game = std::make_unique<Game>(GetDisplayRefreshRate());
    auto game = std::make_unique<Game>(100'000);
    game->Init();
    game->AddScreen(std::make_unique<AniScreen>());
    game->Run(AniScreen::ScreenName());
    return 0;
}
