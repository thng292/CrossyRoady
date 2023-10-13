#include "ConsoleGame.h"
#include "PreviewScreen.h"
#include <memory>
#include <windows.media.playback.h>

using namespace ConsoleGame;

auto main() -> int {
    ABI::Windows::Media::Playback::MediaPlayer tmp();
    auto game = std::make_unique<Game>(1);
    game->Init();
    game->AddScreen(std::make_unique<PreviewScreen>());
    game->Run(PreviewScreen::ScreenName());
    return 0;
}