#include <memory>

#include "ConsoleGame.h"
#include "DevScreen.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    auto game = std::make_unique<Game>(60);
    game->Init();
    game->AddScreen(std::make_unique<DevScreen>());
    game->Run(DevScreen::ScreenName());
    return 0;
}
