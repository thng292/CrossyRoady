#pragma once
#include <chrono>
#include <memory>

#include "ConsoleGame.h"
#include "MenuBG.h"

template <typename Func>
concept MenuSelectedCB = std::is_nothrow_invocable_r_v<void, Func, uint8_t>;

std::string SecondsToHMStr(std::chrono::seconds time);

void DrawRhombus(
    ConsoleGame::AbstractCanvas* canvas,
    ConsoleGame::Vec2 center,
    uint8_t R,
    ConsoleGame::Color color
);
