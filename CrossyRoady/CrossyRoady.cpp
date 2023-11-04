#include <memory>

#include "CharaSelectScreen.h"
#include "ConsoleGame.h"
#include "MainMenu.h"
using namespace ConsoleGame;

const char* configFilePath = "config.txt"; 

auto main() -> int
{
    Font::Load("resource/font/small.font");
    Font::Load("resource/font/big.font", 1);
    LocalStorage::LoadFromFile(configFilePath);
    defer { LocalStorage::SaveToFile(configFilePath); };
    auto game =
        std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<MainMenu>());
    game->Run(MainMenu::ScreenName());
    return 0;
}
