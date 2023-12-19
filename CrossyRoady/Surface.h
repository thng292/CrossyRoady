#pragma once

#include "ConsoleGame.h"

struct SurfaceArgs {
    ConsoleGame::Vec2 size = {0, 0};
    ConsoleGame::Vec2 pos = {0, 0};
    uint8_t cornerSize = 0;
    bool hasBorder = false;
    char background = ConsoleGame::C_TRANSPARENT;
    char border = ConsoleGame::C_TRANSPARENT;
};

class Surface {
   public:
    SurfaceArgs props;

    Surface() = default;
    Surface(SurfaceArgs args);
    bool IsHover(ConsoleGame::Vec2 mousePos) const;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
};
