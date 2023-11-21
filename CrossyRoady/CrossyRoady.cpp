#include <memory>

#include "CharaSelectScreen.h"
#include "CharaUnlock.h"
#include "CharactersInfo.h"
#include "ConsoleGame.h"
#include "Credit.h"
#include "GameMap.h"
#include "HowToPlay.h"
#include "MainMenu.h"
#include "Pause.h"
#include "Progress.h"
#include "Result.h"
#include "Setting.h"
using namespace ConsoleGame;

// #define _TEST_PERF_ 

const char* configFilePath = "config.txt";

auto main() -> int
{
#ifdef _DEBUG
    std::ofstream log("log.txt");
    Logger::Init(&log);
#endif

    Font::Load(RESOURCE_PATH FONT_PATH "small.font");
    Font::Load(RESOURCE_PATH FONT_PATH "big.font", 1);

    R.Config.Load(CONFIG_PATH);
    defer { R.Config.Save(CONFIG_PATH); };

    auto game =
#ifndef _TEST_PERF_ 
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
    game->AddScreen(std::make_unique<Pause>());
    game->AddScreen(std::make_unique<CharaUnlock>());
    game->AddScreen(std::make_unique<Result>());
    game->Run(Result::ScreenName());

    return 0;
}
