#include <memory>

#include "AniScreen.h"
#include "ConsoleGame.h"
#include "DevScreen.h"
using namespace ConsoleGame;

auto main() -> int
{
    int fps = 60;
    {
        // Get display's refresh rate
        DEVMODE devMode;
        devMode.dmSize = sizeof(devMode);
        bool ok = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
        if (ok) {
            fps = devMode.dmDisplayFrequency;
        }
    }
    Font::Load("test.font");
    auto game = std::make_unique<Game>(fps);
    game->Init();
    game->AddScreen(std::make_unique<AniScreen>());
    game->Run(AniScreen::ScreenName());
    return 0;
}
