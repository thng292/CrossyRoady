#pragma once
#include <chrono>
#include <memory>

#include "ConsoleGame.h"
#include "MenuBG.h"

constexpr int LevelExpReq = 200;
constexpr float buttonDelay = 0.3;
constexpr uint8_t numberOfChars = 6;

constexpr std::array<std::string_view, 6> fileCharName = {
    "fauna", "irys", "mumei", "kronii", "sana", "bae"};

constexpr std::array<std::string_view, 6> fileMapName = {
    "forest", "city", "house", "desert", "space", "casino"};


template <typename Func>
concept MenuSelectedCB = std::is_nothrow_invocable_r_v<void, Func, uint8_t>;

std::string SecondsToHMStr(std::chrono::seconds time);

void DrawRhombus(
    ConsoleGame::AbstractCanvas* canvas,
    ConsoleGame::Vec2 center,
    uint8_t R,
    ConsoleGame::Color color
);
