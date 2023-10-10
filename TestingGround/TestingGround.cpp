#include "ConsoleGame.h"
#include "PreviewScreen.h"
#include <memory>

using namespace ConsoleGame;

auto main() -> int {
    auto game = std::make_unique<Game>();
    game->Init();
    game->AddScreen(std::make_unique<PreviewScreen>());
    game->Run(PreviewScreen::ScreenName());
    return 0;
}