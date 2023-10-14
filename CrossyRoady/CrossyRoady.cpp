#include "ConsoleGame.h"
#include "DevScreen.h"
#include <memory>

using namespace ConsoleGame;

auto main() -> int
{
    auto game = std::make_unique<Game>(120);
    game->Init();
    game->AddScreen(std::make_unique<DevScreen>());
    game->Run(DevScreen::ScreenName());
    return 0;
}
