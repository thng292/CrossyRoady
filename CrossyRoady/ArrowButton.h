#pragma once

#include "Surface.h"

class ArrowButton : private Surface {
    bool direction;

   public:
    ArrowButton() = default;
    ArrowButton(SurfaceArgs surfaceArgs, bool direction);

    bool IsHover(ConsoleGame::Vec2 mousePos) const;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
};