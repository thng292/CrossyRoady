#pragma once
#include <chrono>
#include <memory>

#include "ConsoleGame.h"
#include "MenuBG.h"

struct SharedMenuStuff {
    ConsoleGame::Audio* bgMusic = nullptr;
    ConsoleGame::Audio* hoverSfx = nullptr;
    MenuBG* menuBg = nullptr;
};

std::string SecondsToHMStr(std::chrono::seconds time);
