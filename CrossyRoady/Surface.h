#pragma once

#include "ConsoleGame.h"

struct SurfaceArgs {
    ConsoleGame::Vec2 size = {0, 0};
    ConsoleGame::Vec2 pos = {0, 0};
    uint8_t cornerSize = 0;
    bool hasBorder = false;
    ConsoleGame::Color background = ConsoleGame::Color::C_TRANSPARENT;
    ConsoleGame::Color border = ConsoleGame::Color::C_TRANSPARENT;
    ConsoleGame::Color ring = ConsoleGame::Color::C_TRANSPARENT;
};

class Surface {
   public:
    bool ringState = false;
    SurfaceArgs props;

    Surface(SurfaceArgs args);
    bool IsHover(ConsoleGame::Vec2 mousePos);
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
};
