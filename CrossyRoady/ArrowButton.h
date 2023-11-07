#pragma once

#include <string_view>

#include "Surface.h"

class ArrowButton : private Surface {
    std::string_view direction;

   public:
    ArrowButton() = default;
    ArrowButton(SurfaceArgs surfaceArgs, const std::string_view direction);

    bool IsHover(ConsoleGame::Vec2 mousePos) const;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
};