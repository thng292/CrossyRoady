#pragma once
#include <chrono>
#include <memory>

#include "ConsoleGame.h"
#include "MenuBG.h"

constexpr int LevelExpReq = 200;
constexpr float buttonDelay = 0.2;

template <typename Func>
concept MenuSelectedCB = std::is_nothrow_invocable_r_v<void, Func, uint8_t>;

std::string SecondsToHMStr(std::chrono::seconds time);

void DrawRhombus(
    ConsoleGame::AbstractCanvas* canvas,
    ConsoleGame::Vec2 center,
    uint8_t R,
    ConsoleGame::Color color
);
