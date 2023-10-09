#include "ConsoleGame.h"
#include "DevScreen.h"
#include <memory>

using namespace ConsoleGame;

auto main() -> int
{
    auto game = std::make_unique<Game>();
    game->Init();
    game->addScreen(std::make_unique<DevScreen>());
    game->Run(L"DevScreen");
    return 0;
}
