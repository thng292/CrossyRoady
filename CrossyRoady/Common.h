#pragma once
#include <chrono>
#include <memory>

#include "ConsoleGame.h"
#include "MenuBG.h"

constexpr int LevelExpReq = 200;

std::string SecondsToHMStr(std::chrono::seconds time);

void DrawRhombus(
    ConsoleGame::AbstractCanvas* canvas,
    ConsoleGame::Vec2 center,
    uint8_t R,
    ConsoleGame::Color color
);
