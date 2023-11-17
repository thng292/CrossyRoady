#include <memory>

#include "CharaSelectScreen.h"
#include "CharactersInfo.h"
#include "ConsoleGame.h"
#include "Credit.h"
#include "GameMap.h"
#include "HowToPlay.h"
#include "MainMenu.h"
#include "Progress.h"
#include "Setting.h"
using namespace ConsoleGame;

// #define _SHOW_OFF_

const char* configFilePath = "config.txt";

auto main() -> int
{
    std::ofstream log("log.txt");
    Logger::Init(&log);

    Font::Load(RESOURCE_PATH FONT_PATH "small.font");
    Font::Load(RESOURCE_PATH FONT_PATH "big.font", 1);

    R.Config.Load(CONFIG_PATH);
    defer { R.Config.Save(CONFIG_PATH); };

    auto game =
#ifndef _SHOW_OFF_
        std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
#else
        std::make_unique<Game>(L"Crossy Roady", 9999999);
#endif

    game->AddScreen(std::make_unique<MainMenu>());
    game->AddScreen(std::make_unique<Setting>());
    game->AddScreen(std::make_unique<Progress>());
    game->AddScreen(std::make_unique<Credit>());
    game->AddScreen(std::make_unique<GameMap>());
    game->AddScreen(std::make_unique<CharactersInfo>());
    game->AddScreen(std::make_unique<HowToPlay>());
    game->AddScreen(std::make_unique<CharacterSelectScreen>());
    game->Run(MainMenu::ScreenName());

    return 0;
}
