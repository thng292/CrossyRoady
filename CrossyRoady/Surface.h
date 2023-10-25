#pragma once

#include "ConsoleGame.h"

struct SurfaceArgs {
    ConsoleGame::Vec2 size = {0, 0};
    ConsoleGame::Vec2 pos = {0, 0};
    bool roundedCorner = false;
    ConsoleGame::Color background = ConsoleGame::Color::C_TRANSPARENT;
    ConsoleGame::Color border = ConsoleGame::Color::C_TRANSPARENT;
    ConsoleGame::Color ring = ConsoleGame::Color::C_TRANSPARENT;
};

class Surface {
    SurfaceArgs initArgs;

   public:
    bool ringState = false;
    bool borderState = false;

    Surface(SurfaceArgs args);
    bool IsHover(ConsoleGame::Vec2 mousePos);
    void Draw(ConsoleGame::AbstractCanvas* canvas);
};
